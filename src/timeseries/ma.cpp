#include "timeseries/time_series_model.hpp"

MA::MA(const TimeSeries<double>& data) {
    this->data = data;
    this->count = data.size();
    this->name = "MA Model (Untrained)";

    // Mark model as untrained 
    this->maOrder = -1;

    // Calculate mean
    double sum = 0.0;
    for (const auto& [date, value] : data) {
        sum += value;
    }
    this->mean = sum / this->count;
}

MA::~MA() {}

double getNLL(const std::vector<double>& params, const std::vector<double>& data) {
    double mu = params[0];
    size_t count = data.size();
    std::vector<double> maParams(params.begin() + 1, params.end());
    int k = maParams.size()-1;
    std::vector<double> residuals(count, 0.0);
    double sumResidualsSq = 0.0;

    // Get residuals for first k points
    for (size_t i = 0; i < k; ++i) {
        residuals[i] = data[i] - mu;  // Assuming no MA effect for initial points
        sumResidualsSq += residuals[i] * residuals[i];
    }

    // Loop through the data to calculate residuals for the rest of the points
    for (int i = k; i < count; ++i) {
        double prediction = mu;

        // Use past residuals in the MA part
        for (int j = 0; j < k; ++j) {
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

void MA::train(int maOrder) {
    this->maOrder = maOrder;

    nlopt::opt optimizer(nlopt::LN_COBYLA, maOrder+1);
    optimizer.set_xtol_rel(1e-6);
    optimizer.set_maxeval(10000);

    std::vector<double> x(maOrder+1, 0.0);
    double minNLL;

    // Get price vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    try {
        optimizer.set_min_objective(objFunction, &dataVec);
        optimizer.optimize(x, minNLL);
    } catch (const std::exception &e) {
        std::cerr << "nlopt failed: " << e.what() << std::endl;
    }

    // Save learnt parameters
    this->mean = x[0];
    this->thetas.clear();
    for (int i = 1; i < maOrder+1; ++i) {
        this->thetas.push_back(x[i]);
    }

    // Set new name 
    this->name = fmt::format("MA({}) Model", maOrder);
}

void MA::forecast(int steps) {
    int k = this->maOrder;

    std::time_t startDate = this->data.end()->first;
    // Construct data vector
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // Get residuals for current learnt parameters 
    std::vector<double> residuals(this->count);
    // Compute residuals for initial data
    for (int i = k; i < this->count; ++i) {
        double prediction = this->mean;

        // Weighted sum of MA terms
        for (int j = 0; j < k; ++j) {
            prediction += this->thetas[j] * residuals[i - j - 1];
        }
        residuals[i] = dataVec[i] - prediction;
    }

    // Forecast future values
    for (int i = 0; i < steps; ++i) {
        double forecast = this->mean;
        for (int j = 0; j < k; ++j) {
            forecast += this->thetas[j] * residuals[this->count - k + i + j];
        }
        this->forecasted[startDate] = forecast;
        startDate += intervalToSeconds("1d");
        residuals.push_back(0.0); // Assume residuals are 0 for future values
    }
}

std::vector<double> MA::getThetas() const {
    return this->thetas;
}

std::string MA::toString() const {
    std::vector<std::vector<std::string>> tableData;
    if (this->maOrder == -1) {
        // Untrained model
        tableData = {{"", ""}};
    } else {
        // Trained model
        for (int i = 0; i < this->maOrder; ++i) {
            tableData.push_back({fmt::format("theta_{}", i+1), fmt::format("{:.4f}", this->thetas[i])});
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