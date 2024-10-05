#include "timeseries/time_series_model.hpp"

MA::MA(const TimeSeries<double>& data) {
    this->data = data;
    this->count = data.size();
    this->name = "MA Model (Untrained)";

    // Mark model as untrained 
    this->maOrder = -1;
    this->c = 0;

    this->mse = 0.0;
    this->rmse = 0.0;
    this->mae = 0.0;
}

double getNLLMA(const std::vector<double>& params, const std::vector<double>& data) {
    double mu = params[0]; 
    size_t count = data.size();
    std::vector<double> maParams(params.begin() + 1, params.end());
    size_t k = maParams.size(); 
    std::vector<double> residuals(count, 0.0);
    double sumResidualsSq = 0.0;

    // Get residuals for the first k points (no MA effect for the initial points)
    for (size_t i = 0; i < k; ++i) {
        residuals[i] = data[i] - mu;  // Simple difference for initial residuals
        sumResidualsSq += residuals[i] * residuals[i];
    }

    // Calculate residuals for the rest of the data using MA model
    for (size_t i = k; i < count; ++i) {
        double prediction = mu; 
        for (size_t j = 0; j < k; ++j) {
            prediction += maParams[j] * residuals[i - j - 1]; 
        }
        residuals[i] = data[i] - prediction;
        sumResidualsSq += residuals[i] * residuals[i];
    }

    // Estimate variance from the residuals
    double sigmaSq = sumResidualsSq / count; 

    // Compute NLL
    return 0.5 * count * std::log(2 * M_PI * sigmaSq) + (0.5 / sigmaSq) * sumResidualsSq;
}


// Wrapper for NLOpt
double objFunctionMA(const std::vector<double>& x, std::vector<double>& grad, void *data) {
    std::vector<double> *dataPtr = static_cast<std::vector<double>*>(data);
    return getNLLMA(x, *dataPtr);
}

void MA::train(int maOrder) {
    this->maOrder = maOrder;

    nlopt::opt optimizer(nlopt::LN_COBYLA, maOrder+1);
    optimizer.set_xtol_rel(1e-7);
    optimizer.set_maxeval(20000);

    // Get price vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    double sampleMean = std::accumulate(dataVec.begin(), dataVec.end(), 0.0) / dataVec.size();
    std::vector<double> x(maOrder+1, 0.1);
    x[0] = sampleMean;  // Initialize mean parameter with sample mean
    double minNLL;

    try {
        optimizer.set_min_objective(objFunctionMA, &dataVec);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->c = x[0];
    this->thetas.clear();
    for (int i = 1; i < maOrder+1; ++i) {
        this->thetas.push_back(x[i]);
    }

    // Get accuracy metrics 
    Eigen::VectorXd labels(this->count - maOrder);
    Eigen::VectorXd predictions(this->count - maOrder);
    std::vector<double> residuals; 

    // Set simple first maOrder residuals 
    for (int i = 0; i < maOrder; ++i) {
        residuals.push_back(dataVec[i] - this->c);
    }

    for (size_t i = maOrder; i < this->count; ++i) {
        double prediction = this->c;
        for (int j = 0; j < maOrder; ++j) {
            prediction += this->thetas[j] * residuals[residuals.size() - j - 1];
        }
        residuals.push_back(dataVec[i] - prediction);
        labels(i - maOrder) = dataVec[i];
        predictions(i - maOrder) = prediction;
    }

    this->mse = (labels - predictions).squaredNorm() / (this->count - maOrder);
    this->rmse = std::sqrt(this->mse);
    this->mae = (labels - predictions).cwiseAbs().sum() / (this->count - maOrder);

    // Set new name 
    this->name = fmt::format("MA({}) Model", maOrder);
}

void MA::forecast(int steps) {
    this->forecasted.clear();
    int k = this->maOrder;

    std::time_t startDate = this->data.rbegin()->first + intervalToSeconds("1d");

    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // Get residuals for current learnt parameters
    std::vector<double> residuals(this->count);
    for (size_t i = k; i < this->count; ++i) {
        double prediction = this->c;

        // Weighted sum of MA terms
        for (int j = 0; j < k; ++j) {
            prediction += this->thetas[j] * residuals[i - j - 1];
        }
        residuals[i] = dataVec[i] - prediction;
    }

    // Forecast future values
    for (int i = 0; i < steps; ++i) {
        double forecast = this->c;
        
        // Sample a residual from the past residuals (bootstrapping)
        double sampledResidual = residuals[rand() % this->count];
        for (int j = 0; j < k; ++j) {
            forecast += this->thetas[j] * residuals[this->count - k + i + j];
        }

        forecast += sampledResidual;
        this->forecasted[startDate] = forecast;
        startDate += intervalToSeconds("1d");
        residuals.push_back(sampledResidual);
    }
}


std::vector<double> MA::getThetas() const {
    return this->thetas;
}

std::string MA::toString() const {
    auto columnWidths = {12, 12};
    int totalWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0) + columnWidths.size() - 1;
    auto justifications = {Justification::LEFT, Justification::RIGHT};
    auto colors = {Color::WHITE, Color::WHITE};

    // Title 
    auto table = getTopLine({totalWidth});
    table += getRow({this->name}, {totalWidth}, {Justification::CENTER}, {Color::WHITE});

    // Params
    table += getMidLine({columnWidths}, Ticks::LOWER);
    for (int i = 0; i < this->maOrder; ++i) {
        table += getRow({fmt::format("theta_{}", i + 1), fmt::format("{:.4f}", this->thetas[i])}, columnWidths, justifications, colors);
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