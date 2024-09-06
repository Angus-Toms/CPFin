#include "timeseries/ma.hpp"

MA::MA(const std::vector<double> data) {
    this->data = data;
    this->count = data.size();

    // Calculate mean 
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
}

MA::~MA() {}

double getNLL(const std::vector<double>& params, const std::vector<double>& data) {
    double mu = params[0];
    double theta = params[1];
    size_t count = data.size();

    // Calculate residuals and their variance 
    std::vector<double> residuals(count);
    double sumResidualsSq = 0.0;

    // Compute residuals 
    for (size_t i = 1; i < count; ++i) {
        residuals[i] = data[i] - mu - theta * (data[i-1] - mu);
        sumResidualsSq += residuals[i] * residuals[i];
    }

    double sigmaSq = sumResidualsSq / count; // Variance 

    // Calculate negative log-likelihood
    return (0.5 * std::log(2 * M_PI * sigmaSq)) + (0.5 / sigmaSq) * sumResidualsSq;
}

// Wrapper for NLOpt
double objFunction(const std::vector<double>& x, std::vector<double>& grad, void *data) {
    std::vector<double> *dataPtr = static_cast<std::vector<double>*>(data);
    return getNLL(x, *dataPtr);
}

void MA::train() {
    int paramCount = 2;
    nlopt::opt optimizer(nlopt::LN_COBYLA, paramCount);
    optimizer.set_xtol_rel(1e-6);
    optimizer.set_maxeval(10000);

    std::vector<double> x(paramCount, 0.0);  // Initial values
    double minNLL; 

    try {
        optimizer.set_min_objective(objFunction, &this->data);
        optimizer.optimize(x, minNLL);
        std::cout << "Optimal parameters: " << x[0] << ", " << x[1] << std::endl;
        std::cout << "Minimum NLL: " << minNLL << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }
}

std::vector<double> MA::forecast(int steps) const {
    return {};
}

std::vector<double> MA::getThetas() const {
    return {};
}

int MA::plot() const {
    return 0;
}

std::string MA::toString() const {
    return "MA(1) model";
}