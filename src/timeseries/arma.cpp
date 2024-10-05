#include "timeseries/timeseries_models.hpp"

struct ARMAData {
    std::vector<double> data;
    int p;
    int q;
};

ARMA::ARMA(const TimeSeries<double>& data) {
    this->data = data;
    this->count = data.size();
    this->name = "ARMA Model (Untrained)";

    // Mark model as untrained
    this->arOrder = -1;
    this->maOrder = -1;

    this->c = 0;

    this->mse = 0;
    this->rmse = 0;
    this->mae = 0;
}

double getNLLARMA(const std::vector<double>& params, const std::vector<double>& data, int p, int q) {
    double mu = params[0];
    std::vector<double> arCoeffs(params.begin() + 1, params.begin() + p + 2);
    std::vector<double> maCoeffs(params.begin() + p + 1, params.begin() + p + q + 1);

    size_t count = data.size();
    std::vector<double> residuals(count, 0.0);
    double sumResidualsSq = 0.0;

    for (size_t t = std::max(p, q); t < count; ++t) {
        double arPart = 0.0;
        double maPart = 0.0;

        // X_t = phi_1 * X_{t-1} + ... + phi_p * X_{t-p} + e_t + theta_1 * e_{t-1} + ... + theta_q * e_{t-q}
        // arPart = phi_1 * X_{t-1} + ... + phi_p * X_{t-p}
        for (int i = 0; i < p; ++i) {
            arPart += arCoeffs[i] * data[t - 1 - i];
        }

        // maPart = theta_1 * e_{t-1} + ... + theta_q * e_{t-q}
        for (int i = 0; i < q; ++i) {
            maPart += maCoeffs[i] * residuals[t - 1 - i];
        }

        residuals[t] = data[t] - arPart - maPart - mu;
        sumResidualsSq += residuals[t] * residuals[t];
    }

    double sigmaSq = sumResidualsSq / (count - std::max(p, q));
    double nll = 0.5 * (count - std::max(p, q)) * std::log(2 * M_PI * sigmaSq) + (0.5 / sigmaSq) * sumResidualsSq;
    return nll;
}

double objFunctionARMA(const std::vector<double>& x, std::vector<double>& grad, void *data) {
    ARMAData* modelData = static_cast<ARMAData*>(data);
    return getNLLARMA(x, modelData->data, modelData->p, modelData->q);
}

void ARMA::train(int arOrder, int maOrder) {
    this->arOrder = arOrder;
    this->maOrder = maOrder;

    int paramCount = arOrder + maOrder + 1; // Include AR, MA params and mean
    nlopt::opt optimizer(nlopt::LN_COBYLA, paramCount);
    optimizer.set_xtol_rel(1e-6);
    optimizer.set_maxeval(10000);

    // Construct data vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // Set initial mean param to empirical mean
    std::vector<double> x(paramCount, 0.1);
    x[0] = std::accumulate(dataVec.begin(), dataVec.end(), 0.0) / dataVec.size();
    double minNLL;

    try {
        // Data wrapper to pass model order to objective function
        ARMAData armaData = {dataVec, arOrder, maOrder};
        optimizer.set_min_objective(objFunctionARMA, &armaData);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->c = x[0];
    this->phis.clear();
    for (int i = 1; i < this->arOrder+1; ++i) {
        this->phis.push_back(x[i]);
    }
    this->thetas.clear();
    for (int i = 1; i < this->maOrder+1; ++i) {
        this->thetas.push_back(x[i+this->arOrder]);
    }

    // Set new name
    this->name = fmt::format("ARMA({}, {}) Model", arOrder, maOrder);

    // Set metrics 
    // First maOrder residuals are simple
    std::vector<double> residuals;
    for (int i = 0; i < maOrder; ++i) {
        residuals.push_back(dataVec[i] - this->c);
    }

    // Make predictions on training data
    Eigen::VectorXd labels(this->count - std::max(arOrder, maOrder));
    Eigen::VectorXd predictions(this->count - std::max(arOrder, maOrder));
    for (size_t i = std::max(arOrder, maOrder); i < this->count; ++i) {
        labels(i - std::max(arOrder, maOrder)) = dataVec[i];
        double prediction = this->c;
        for (int j = 0; j < arOrder; ++j) {
            prediction += this->phis[j] * dataVec[dataVec.size() - j - 1];
        }
        for (int j = 0; j < maOrder; ++j) {
            prediction += this->thetas[j] * residuals[residuals.size() - j - 1];
        }
        predictions(i - std::max(arOrder, maOrder)) = prediction;
        dataVec.push_back(prediction);
        residuals.push_back(dataVec[i] - prediction);
    }

    this->mse = (labels - predictions).squaredNorm() / (this->count - std::max(arOrder, maOrder));
    this->rmse = std::sqrt(this->mse);
    this->mae = (labels - predictions).cwiseAbs().sum() / (this->count - std::max(arOrder, maOrder));
}

void ARMA::forecast(int steps) {
    this->forecasted.clear();
    int p = this->arOrder;
    int q = this->maOrder;
    size_t count = this->count;

    std::vector<double> residuals(count);

    // Starting date for predictions
    std::time_t startDate = this->data.rbegin()->first + intervalToSeconds("1d");

    // Construct data vector from historical data
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // Get residuals for current learned parameters
    for (int t = std::max(p, q); t < count; ++t) {
        double arPart = 0.0;
        double maPart = 0.0;

        // AR part: phi_1 * X_{t-1} + ... + phi_p * X_{t-p}
        for (int i = 0; i < p; ++i) {
            arPart += this->phis[i] * dataVec[t - i - 1];
        }

        // MA part: theta_1 * e_{t-1} + ... + theta_q * e_{t-q}
        for (int i = 0; i < q; ++i) {
            maPart += this->thetas[i] * residuals[t - i - 1];
        }

        double prediction = arPart + maPart + this->c;
        residuals[t] = dataVec[t] - prediction;
    }

    // Forecast future values
    for (int i = 0; i < steps; ++i) {
        double forecast = this->c;
        double arPart = 0.0;
        double maPart = 0.0;

        // AR part
        for (int j = 0; j < p; ++j) {
            arPart += this->phis[j] * dataVec[count + i - j - 1]; 
        }

        // MA part: Bootstrap residuals from past residuals
        double sampledResidual = residuals[rand() % count];
        for (int k = 0; k < q; ++k) {
            maPart += this->thetas[k] * residuals[count + i - k - 1]; 
        }

        forecast += (arPart + maPart);
        this->forecasted[startDate] = forecast;
        startDate += intervalToSeconds("1d");
        dataVec.push_back(forecast);
        residuals.push_back(sampledResidual);
    }
}

std::vector<double> ARMA::getPhis() const {
    return this->phis;
}

std::vector<double> ARMA::getThetas() const {
    return this->thetas;
}

std::string ARMA::toString() const {
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