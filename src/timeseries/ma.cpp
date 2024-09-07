#include "timeseries/ma.hpp"

MA::MA(const std::vector<double> data) {
    this->data = data;
    this->count = data.size();
}

MA::~MA() {}

double getNLL(const std::vector<double>& params, const std::vector<double>& data) {
    // Updated NLL for multi-theta models
    double mu = params[0];
    size_t n = params.size() - 1;
    size_t count = data.size();
    std::vector<double> arParams(params.begin() + 1, params.end());

    std::vector<double> residuals(count);
    double sumResidualsSq = 0.0;

    // Get residuals 
    for (size_t i = n; i < count; ++i) {
        double prediction = mu; 

        // Add AR terms 
        for (size_t j = 0; j < n; ++j) {
            prediction += arParams[j] * data[i-j-1];
        }

        residuals[i] = data[i] - prediction;
        sumResidualsSq += residuals[i] * residuals[i];
    }

    double sigmaSq = sumResidualsSq / (count-n);

    double nll = (0.5 * std::log(2 * M_PI * sigmaSq)) + (0.5 / sigmaSq) * sumResidualsSq;
    return nll;
}

// Wrapper for NLOpt
double objFunction(const std::vector<double>& x, std::vector<double>& grad, void *data) {
    std::vector<double> *dataPtr = static_cast<std::vector<double>*>(data);
    return getNLL(x, *dataPtr);
}

void MA::train(int k) {
    this->k = k;

    int paramCount = k+1; // Include mean and k theta params
    nlopt::opt optimizer(nlopt::LN_COBYLA, paramCount);
    optimizer.set_xtol_rel(1e-6);
    optimizer.set_maxeval(10000);

    std::vector<double> x(paramCount, 0.0);
    double minNLL;

    try {
        optimizer.set_min_objective(objFunction, &this->data);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->mean = x[0];
    this->theta.clear();
    for (int i = 1; i < paramCount; ++i) {
        this->theta.push_back(x[i]);
    }
}

std::vector<double> MA::forecast(int steps) const {
    return {};
}

std::vector<double> MA::getThetas() const {
    return this->theta;
}

int MA::plot() const {
    return 0;
}

std::string MA::toString() const {
    return "MA(1) model";
}
