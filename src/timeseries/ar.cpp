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
    // X_t = mu + phi_1 * X_t-1 + phi_2 * X_t-2 + ... + phi_k * X_t-k
    Eigen::MatrixXd features(this->count - k, k + 1); // Add column for mean
    Eigen::VectorXd labels(this->count - k);
    for (size_t i = 0; i < this->count - k; ++i) {
        for (int j = 0; j < k; ++j) {
            features(i, j) = this->data[i + k - j - 1]; // Reverse the order of historical values
        }
        features(i, k) = 1.0; // Column for the constant term (mean)
        labels(i) = this->data[i + k];
    }

    Eigen::VectorXd phis = features.colPivHouseholderQr().solve(labels);
    this->phis.clear();
    // Add 1 to account for mean
    for (int i = 0; i < k + 1; ++i) {
        this->phis.push_back(phis(i));
    }
}

std::vector<double> AR::forecast(int steps) const {
    int k = this->k;
    std::vector<double> forecasted;

    // X_t = mu + (phi_1 * X_{t-1}) + (phi_2 * X_{t-2}) + ... + (phi_k * X_{t-k})
    for (int i = 0; i < steps; ++i) {
        double forecast = this->mean;
        for (int j = 0; j < k; ++j) { 
            double dataPoint = i + j < k ?
                this->data[this->count - k + i + j] :
                forecasted[i - k + j];
            forecast += this->phis[j] * (dataPoint - this->mean);
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