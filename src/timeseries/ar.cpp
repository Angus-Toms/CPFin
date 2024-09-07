#include "timeseries/ar.hpp"

AR::AR(const std::vector<double> data) {
    this->data = data;
    this->count = data.size();

    // Calculate mean
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    this->mean = sum / this->count;
}

AR::~AR() {}

void AR::train(int k) {
    this->k = k;

    // Construct feature matrix and label vector
    // X_t = phi_1 * X_t-1 + phi_2 * X_t-2 + ... + phi_k * X_t-k
    Eigen::MatrixXd features(this->count - k, k);
    Eigen::VectorXd labels(this->count - k);
    for (size_t i = 0; i < this->count - k; ++i) {
        for (int j = 0; j < k; ++j) {
            features(i, j) = this->data[i + j];
        }
        labels(i) = this->data[i + k];
    }

    Eigen::VectorXd phis = features.colPivHouseholderQr().solve(labels);
    this->phis.clear();
    for (int i = 0; i < k; ++i) {
        this->phis.push_back(phis(i));
    }
}

std::vector<double> AR::forecast(int steps) const {
    int k = this->phis.size();
    std::vector<double> forecasted;

    for (int i = 0; i < steps; ++i) {
        double forecast = 0.0;
        for (int j = 0; j < k; ++j) {
            // If we are forecasting in the first k steps, use past data 
            if (i+j < k) {
                forecast += this->data[this->count - k + i + j] * this->phis[j];
            } else {
                forecast += forecasted[i + j - k] * this->phis[j];
            }
        }
        forecasted.push_back(forecast);
    }
    return forecasted;
}

std::vector<double> AR::getPhis() const {
    return this->phis;
}

std::vector<double> AR::getAutocorrelations() const {
    return this->autocorrelations;
}

int AR::plot() const {
    return 0;
}

std::string AR::toString() const {
    return "";
}