#include "timeseries/time_series_model.hpp"

AR::AR(const TimeSeries<double>& data) {
    this->data = data;
    this->count = data.size();
    this->arOrder = -1; // Mark model as untrained
    this->name = "AR Model (Untrained)";

    double sum = 0;
    for (const auto& [date, value] : this->data) {
        sum += value;
    }
    this->mean = sum / this->count;
}

void AR::train(int arOrder) {
    this->arOrder = arOrder;
    std::vector<double> dataVec;
    
    // Extract values from data
    for (const auto& [date, value] : this->data) {
        dataVec.push_back(value);
    }

    // Construct feature matrix and label vector
    // Add a column for the constant term (mean)
    Eigen::MatrixXd features(this->count - arOrder, arOrder + 1); // +1 for the mean term
    Eigen::VectorXd labels(this->count - arOrder);

    for (size_t i = 0; i < this->count - arOrder; ++i) {
        // Fill feature matrix with historical values
        for (int j = 0; j < arOrder; ++j) {
            features(i, j) = dataVec[i + arOrder - j - 1]; // Reverse the order of historical values
        }
        features(i, arOrder) = 1.0; // Column for the constant term (mean)
        labels(i) = dataVec[i + arOrder];
    }

    // Solve for coefficients using least squares
    Eigen::VectorXd coefficients = features.colPivHouseholderQr().solve(labels);
    
    // Extract mean and AR coefficients
    this->mean = coefficients(arOrder);
    this->phis.assign(coefficients.data(), coefficients.data() + arOrder);

    // Calculate accuracy metrics
    Eigen::VectorXd residuals = labels - features * coefficients;
    this->mse = residuals.squaredNorm() / residuals.size();
    this->rmse = std::sqrt(this->mse);
    this->mae = residuals.cwiseAbs().sum() / residuals.size();

    // Set new name
    this->name = fmt::format("AR({}) Model", arOrder);
}

void AR::forecast(int steps) {
    this->forecasted.clear();

    int arOrder = this->arOrder;
    std::time_t startDate = this->data.rbegin()->first + intervalToSeconds("1d");
    
    // Get last arOrder values from data
    std::vector<double> history;
    auto it = this->data.end();
    std::advance(it, -arOrder);
    while (it != this->data.end()) {
        history.push_back(it->second);
        ++it;
    }

    // Forecast for the specified number of steps
    for (int i = 0; i < steps; ++i) {
        double forecast = this->mean;
        // X_t = phi_1 * X_{t-1} + phi_2 * X_{t-2} + ... + phi_k * X_{t-k}
        for (int j = 0; j < arOrder; ++j) {
            forecast += this->phis[j] * (history[history.size() - j - 1] - this->mean); 
        }

        // Store the forecasted value and update the history
        history.push_back(forecast);
        this->forecasted[startDate] = forecast;
        startDate += intervalToSeconds("1d");
    }
}

std::vector<double> AR::getPhis() const {
    return this->phis;
}

std::string AR::toString() const {
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
    table += getRow({"Mean", fmt::format("{:.4f}", this->mean)}, columnWidths, justifications, colors);

    // Metrics
    table += getMidLine({columnWidths}, Ticks::BOTH);
    table += getRow({"MSE", fmt::format("{:.4f}", this->mse)}, columnWidths, justifications, colors);
    table += getRow({"RMSE", fmt::format("{:.4f}", this->rmse)}, columnWidths, justifications, colors);
    table += getRow({"MAE", fmt::format("{:.4f}", this->mae)}, columnWidths, justifications, colors);
    table += getBottomLine(columnWidths);

    return table;
}