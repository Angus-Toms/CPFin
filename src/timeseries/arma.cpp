#include "timeseries/time_series_model.hpp"

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
}

ARMA::~ARMA() {}

double getArmaNLL(const std::vector<double>& params, const std::vector<double>& data, int p, int q) {
    std::vector<double> arCoeffs(params.begin(), params.begin() + 1 + p);
    std::vector<double> maCoeffs(params.begin() + p, params.begin() + p + q);

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

        residuals[t] = data[t] - arPart - maPart;
        sumResidualsSq += residuals[t] * residuals[t];
    }

    double sigmaSq = sumResidualsSq / (count - std::max(p, q));
    double nll = 0.5 * (count - std::max(p, q)) * std::log(2 * M_PI * sigmaSq) + (0.5 / sigmaSq) * sumResidualsSq;
    return nll;
}


double objFunctionArma(const std::vector<double>& x, std::vector<double>& grad, void *data) {
    ARMAData* modelData = static_cast<ARMAData*>(data);
    return getArmaNLL(x, modelData->data, modelData->p, modelData->q);
}

void ARMA::train(int arOrder, int maOrder) {
    this->arOrder = arOrder;
    this->maOrder = maOrder;

    int paramCount = arOrder + maOrder; // Include AR and MA params
    nlopt::opt optimizer(nlopt::LN_COBYLA, paramCount);
    optimizer.set_xtol_rel(1e-6);
    optimizer.set_maxeval(10000);

    std::vector<double> x(paramCount, 0.0);
    double minNLL;

    // Construct data vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    try {
        // Data wrapper to pass model order to objective function
        ARMAData armaData = {dataVec, arOrder, maOrder};
        optimizer.set_min_objective(objFunctionArma, &armaData);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->phis.clear();
    for (int i = 0; i < this->arOrder; ++i) {
        this->phis.push_back(x[i]);
    }
    this->thetas.clear();
    for (int i = 0; i < this->maOrder; ++i) {
        this->thetas.push_back(x[i+this->arOrder]);
    }

    // Set new name
    this->name = fmt::format("ARMA({}, {}) Model", arOrder, maOrder);
}

void ARMA::forecast(int steps) {
    int p = this->arOrder;
    int q = this->maOrder;
    size_t count = this->count;
    std::vector<double> residuals(count);

    // Starting date for predictions 
    std::time_t startDate = this->data.end()->first;

    // Construct data vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }


    // Get residuals for current learnt parameters
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

        residuals[t] = dataVec[t] - arPart - maPart;
    }

    // Forecast future values
    for (int i = 0; i < steps; ++i) {
        double forecast = 0.0;
        double arPart = 0.0;
        double maPart = 0.0;

        // AR part, conditionally use past data or forecasted values
        for (int j = 0; j < p; ++j) {
            double dataPoint = i + j < p ?
                dataVec[count - p + i + j] :
                forecasted[i - p + j];
            arPart += this->phis[j] * dataPoint;
        }

        // MA part, use past residuals, assume 0 for future prediction residuals
        for (int j = 0; j < q; ++j) {
            double residual = i + j < q ? 
                residuals[count - q + i + j] :
                0.0;
            maPart += this->thetas[j] * residual;
        }

        forecast = arPart + maPart;
        this->forecasted[startDate] = forecast;
        startDate += intervalToSeconds("1d");
    }
}


std::vector<double> ARMA::getPhis() const {
    return this->phis;
}

std::vector<double> ARMA::getThetas() const {
    return this->thetas;
}

std::string ARMA::toString() const {
    std::vector<std::vector<std::string>> tableData;
    if (this->arOrder == -1 || this->maOrder == -1) {
        // Untrained model 
        tableData = {{"", ""}};
    } else {
        // Trained model
        for (int i = 0; i < this->arOrder; ++i) {
            tableData.push_back({fmt::format("phi_{}", i + 1), fmt::format("{:.4f}", this->phis[i])});
        }
        for (int i = 0; i < this->maOrder; ++i) {
            tableData.push_back({fmt::format("theta_{}", i + 1), fmt::format("{:.4f}", this->thetas[i])});
        }
    }

    return getTable(
        this->name,
        tableData,
        {12, 12},
        {"Parameter", "Value"},
        false
    );
}