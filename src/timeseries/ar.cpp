#include "timeseries/timeseries_models.hpp"

AR::AR(const TimeSeries<double>& data) {
    this->data = data;
    this->count = data.size();
    this->arOrder = -1; // Mark model as untrained
    this->name = "AR Model (Untrained)";
    this->c = 0.0;

    this->mse = 0.0;
    this->rmse = 0.0;
    this->mae = 0.0;
}

double getNLLAR(const std::vector<double>& params, const std::vector<double>& data) {
    double mu = params[0]; // Mean
    size_t count = data.size();
    size_t p = params.size() - 1; // AR order
    std::vector<double> residuals;
    double sumSqResiduals = 0.0;

    // Calculate predictions and residuals with current parameters 
    for (size_t i = p; i < count; ++i) {
        double prediction = mu;
        for (size_t j = 0; j < p; ++j) {
            prediction += params[j + 1] * data[i - j - 1];
        }
        double residual = data[i] - prediction;
        residuals.push_back(residual);
        sumSqResiduals += residual * residual;
    }

    // Get NLL
    double sigmaSq = sumSqResiduals / (count - p);
    return 0.5 * (count - p) * std::log(2 * M_PI * sigmaSq) + (0.5 / sigmaSq) * sumSqResiduals; 
}

// Wrapper for NLOpt
double objFunctionAR(const std::vector<double>& x, std::vector<double>& grad, void *data) {
    std::vector<double>* dataPtr = static_cast<std::vector<double>*>(data);
    return getNLLAR(x, *dataPtr);
}

void AR::train(int arOrder) {
    this->arOrder = arOrder;

    nlopt::opt optimizer(nlopt::LN_COBYLA, arOrder + 1);
    optimizer.set_xtol_rel(1e-7);
    optimizer.set_maxeval(20000);

    // Get price vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    double sampleMean = std::accumulate(dataVec.begin(), dataVec.end(), 0.0) / dataVec.size();
    std::vector<double> x(arOrder + 1, 0.1);
    x[0] = sampleMean;  // Initialize constant parameter to sample mean
    double minNLL;

    try {
        optimizer.set_min_objective(objFunctionAR, &dataVec);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->c = x[0];
    this->phis.clear();
    for (int i = 1; i < arOrder + 1; ++i) {
        this->phis.push_back(x[i]);
    }

    // Get accuracy metrics 
    Eigen::VectorXd labels(this->count - arOrder);
    Eigen::VectorXd predictions(this->count - arOrder);
    for (size_t i = arOrder; i < this->count; ++i) {
        labels(i - arOrder) = dataVec[i];
        double prediction = this->c;
        for (int j = 0; j < arOrder; ++j) {
            prediction += this->phis[j] * dataVec[i - j - 1];
        }
        predictions(i - arOrder) = prediction;
    }
    this->mse = (labels - predictions).squaredNorm() / (this->count - arOrder);
    this->rmse = std::sqrt(this->mse);
    this->mae = (labels - predictions).cwiseAbs().sum() / (this->count - arOrder);

    // Set new name 
    this->name = fmt::format("AR({}) Model", arOrder);
}

void AR::forecast(int steps) {
    this->forecasted.clear();
    std::time_t startDate = this->data.rbegin()->first + intervalToSeconds("1d");

    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    for (int i = 0; i < steps; ++i) {
        double prediction = this->c;
        // Add weighted sum of past values
        for (int j = 0; j < this->arOrder; ++j) {
            prediction += this->phis[j] * dataVec[dataVec.size() - j - 1];
        }
        this->forecasted[startDate + i * intervalToSeconds("1d")] = prediction;
        dataVec.push_back(prediction);
    }
}

std::vector<double> AR::getPhis() const {
    return this->phis;
}

std::string AR::toString() const {
    auto columnWidths = {12, 12};
    int totalWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0) + columnWidths.size() - 1;
    auto justifications = {Justification::LEFT, Justification::RIGHT};
    auto colors = {Color::WHITE, Color::WHITE};

    // Title
    auto table = getTopLine({totalWidth});
    table += getRow({this->name}, {totalWidth}, {Justification::CENTER}, {Color::WHITE});

    // Params 
    table += getMidLine({columnWidths}, Ticks::LOWER);
    for (int i = 0; i < this->arOrder; ++i) {
        table += getRow({fmt::format("phi_{}", i + 1), fmt::format("{:.4f}", this->phis[i])}, columnWidths, justifications, colors);
    }
    table += getRow({"const", fmt::format("{:.4f}", this->c)}, columnWidths, justifications, colors);

    // Metrics
    table += getMidLine({columnWidths}, Ticks::BOTH);
    table += getRow({"MSE", fmt::format("{:.4f}", this->mse)}, columnWidths, justifications, colors);
    table += getRow({"RMSE", fmt::format("{:.4f}", this->rmse)}, columnWidths, justifications, colors);
    table += getRow({"MAE", fmt::format("{:.4f}", this->mae)}, columnWidths, justifications, colors);
    table += getBottomLine(columnWidths);

    return table;
}