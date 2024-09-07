#include "timeseries/arma.hpp"

ARMA::ARMA(const std::vector<double> data) {
    this->data = data;
    this->count = data.size();

    // Calculate mean
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    this->mean = sum / this->count;
}

ARMA::~ARMA() {}

double getArmaNLL(const std::vector<double>& params, const std::vector<double>& data, int p, int q) {
    double mu = params[0];
    std::vector<double> arCoeffs(params.begin() + 1, params.begin() + 1 + p);
    std::vector<double> maCoeffs(params.begin() + 1 + p, params.begin() + 1 + p + q);

    size_t count = data.size();
    std::vector<double> residuals(count, 0.0);
    double sumResidualsSq = 0.0;

    for (size_t t = std::max(p, q); t < count; ++t) {
        double arPart = 0.0;
        double maPart = 0.0;

        // X_t = mu + phi_1 * X_{t-1} + ... + phi_p * X_{t-p} + e_t + theta_1 * e_{t-1} + ... + theta_q * e_{t-q}
        // arPart = phi_1 * X_{t-1} + ... + phi_p * X_{t-p}
        for (int i = 0; i < p; ++i) {
            arPart += arCoeffs[i] * (data[t - 1 - i] - mu);
        }

        // maPart = theta_1 * e_{t-1} + ... + theta_q * e_{t-q}
        for (int i = 0; i < q; ++i) {
            maPart += maCoeffs[i] * residuals[t - 1 - i];
        }

        residuals[t] = data[t] - mu - arPart - maPart;
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

    int paramCount = arOrder + maOrder + 1; // Include mean, AR and MA params
    nlopt::opt optimizer(nlopt::LN_COBYLA, paramCount);
    optimizer.set_xtol_rel(1e-6);
    optimizer.set_maxeval(10000);

    std::vector<double> x(paramCount, 0.0);
    double minNLL;

    try {
        ARMAData armaData = {this->data, arOrder, maOrder};
        optimizer.set_min_objective(objFunctionArma, &armaData);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->mean = x[0];
    this->phis.clear();
    for (int i = 0; i < this->arOrder; ++i) {
        this->phis.push_back(x[i+1]);
    }
    this->thetas.clear();
    for (int i = 0; i < this->maOrder; ++i) {
        this->thetas.push_back(x[i+1+this->arOrder]);
    }
}

std::vector<double> ARMA::forecast(int steps) const {
    return {};
}

double ARMA::getMean() const {
    return this->mean;
}

std::vector<double> ARMA::getPhis() const {
    return this->phis;
}

std::vector<double> ARMA::getThetas() const {
    return this->thetas;
}

int ARMA::plot() const {
    return 0;
}

std::string ARMA::toString() const {
    return "ARMA model";
}