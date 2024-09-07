#include "timeseries/ma.hpp"

MA::MA(const std::vector<double> data) {
    this->data = data;
    this->count = data.size();
}

MA::~MA() {}

double getNLL(const std::vector<double>& params, const std::vector<double>& data) {
    double mu = params[0];
    size_t k = params.size() - 1;
    size_t count = data.size();
    std::vector<double> maParams(params.begin() + 1, params.end());

    std::vector<double> residuals(count, 0.0);
    double sumResidualsSq = 0.0;

    // Get residuals for first k points
    for (size_t i = 0; i < k; ++i) {
        residuals[i] = data[i] - mu;  // Assuming no MA effect for initial points
        sumResidualsSq += residuals[i] * residuals[i];
    }

    // Loop through the data to calculate residuals for the rest of the points
    for (size_t i = k; i < count; ++i) {
        double prediction = mu;

        // Use past residuals in the MA part
        for (size_t j = 0; j < k; ++j) {
            prediction += maParams[j] * residuals[i - j - 1];
        }

        // Calculate residual for the current time step
        residuals[i] = data[i] - prediction;
        sumResidualsSq += residuals[i] * residuals[i];
    }

    // Estimate variance
    double sigmaSq = sumResidualsSq / (count - k);

    // Calculate Negative Log Likelihood (NLL)
    double nll = 0.5 * count * std::log(2 * M_PI * sigmaSq) + (0.5 / sigmaSq) * sumResidualsSq;
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
    int k = this->k;
    std::vector<double> forecasted;

    // Get residuals for current learnt parameters 
    std::vector<double> residuals(this->count);
    // Compute residuals for initial data
    for (int i = k; i < this->count; ++i) {
        double prediction = this->mean;

        // Weighted sum of MA terms
        for (int j = 0; j < k; ++j) {
            prediction += this->theta[j] * residuals[i - j - 1];
        }
        residuals[i] = this->data[i] - prediction;
    }

    // Forecast future values
    for (int i = 0; i < steps; ++i) {
        double forecast = this->mean;
        for (int j = 0; j < k; ++j) {
            forecast += this->theta[j] * residuals[this->count - k + i - j];
            std::cout << "Adding residual: " << residuals[this->count - k + i - j] << " with weight: " << this->theta[j] << std::endl;
        }
        forecasted.push_back(forecast);
        residuals.push_back(0.0); // Assume residuals are 0 for future values
    }

    return forecasted;
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
