#include "timeseries/time_series_model.hpp"

AR::AR(const TimeSeries<double>& data) {
    this->data = data;
    this->count = data.size();
    this->arOrder = -1; // Mark model as untrained
    this->name = "AR Model (Untrained)";

    // Calculate mean
    double sum = 0.0;
    for (const auto& [date, value] : data) {
        sum += value;
    }
    this->mean = sum / this->count;
}

void AR::train(int arOrder) {
    this->arOrder = arOrder;
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // Construct feature matrix and label vector
    // X_t = mu + phi_1 * X_t-1 + phi_2 * X_t-2 + ... + phi_k * X_t-k
    Eigen::MatrixXd features(this->count - arOrder, arOrder + 1); // Add column for mean
    Eigen::VectorXd labels(this->count - arOrder);

    for (size_t i = 0; i < this->count - arOrder; ++i) {
        for (int j = 0; j < arOrder; ++j) {
            features(i, j) = dataVec[i + arOrder - j - 1]; // Reverse the order of historical values

        }
        features(i, arOrder) = 1.0; // Column for the constant term (mean)
        labels(i) = dataVec[i + arOrder];
    }

    Eigen::VectorXd phis = features.colPivHouseholderQr().solve(labels);
    this->phis.clear();
    // Add 1 to account for mean
    for (int i = 0; i < arOrder + 1; ++i) {
        this->phis.push_back(phis(i));
    }

    // Set new name
    this->name = fmt::format("AR({}) Model", arOrder);
}

void AR::forecast(int steps) {
    int arOrder = this->arOrder;
    std::time_t startDate = this->data.end()->first;
    std::vector<double> dataVec;
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // X_t = mu + (phi_1 * X_{t-1}) + (phi_2 * X_{t-2}) + ... + (phi_k * X_{t-k})
    for (int i = 0; i < steps; ++i) {
        double forecast = this->mean;
        for (int j = 0; j < arOrder; ++j) { 
            double dataPoint = i + j < arOrder ?
                dataVec[this->count - arOrder + i + j] :
                forecasted[i - arOrder + j];
            forecast += this->phis[j] * (dataPoint - this->mean);
        }
        // Set new forecast 
        this->forecasted[startDate] = forecast;
        startDate += intervalToSeconds("1d");
    }
}

std::vector<double> AR::getPhis() const {
    return this->phis;
}

std::string AR::toString() const {
    std::vector<std::vector<std::string>> tableData;
    if (this->arOrder == -1) {
        // Untrained model 
        tableData = {{"", ""}};
    } else {
        // Trained model 
        tableData = {{"Mean", fmt::format("{:.4f}", this->phis[this->arOrder])}};
        for (int i = 0; i < this->arOrder; ++i) {
            tableData.push_back({fmt::format("phi_{}", i + 1), fmt::format("{:.4f}", this->phis[i])});
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