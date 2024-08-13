#include "overlays/bollinger.hpp"

BollingerBands::BollingerBands(PriceSeries& priceSeries, int period, double numStdDev, const std::string& maType) 
    : period(period), numStdDev(numStdDev), maType(maType) {
    ps = std::make_unique<PriceSeries>(priceSeries);

    // Set table printing values
    name = fmt::format("{}: BB({}d, {}σ, {})", ps->getTicker(), period, numStdDev, maType);
    columnHeaders = {"Date", "Upper Band", "Middle Band", "Lower Band"};
    columnWidths = {12, 10, 10, 10};

    checkArguments();
    calculate();
}

void BollingerBands::checkArguments() {
}

void BollingerBands::calculate() {
    // TODO: Work out where to get MAs from 
}

void BollingerBands::plot() const {
    namespace plt = matplotlibcpp;

    std::vector<std::time_t> xs;
    std::vector<double> lows;
    std::vector<double> mids;
    std::vector<double> uppers;
    for (const auto& [low, mid, upper] : data) {
        lows.push_back(low);
        mids.push_back(mid);
        uppers.push_back(upper);
    }

    plt::named_plot("Low", xs, lows, "r-");
    plt::named_plot("Middle", xs, mids, "g");
    plt::named_plot("Upper", xs, uppers, "r-");
}

std::vector<std::vector<std::string>> getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, [low, mid, high]] : data) {
        tableData.push_back({
            fmt::format("{:%Y-%m-%d}", date),
            fmt::format("{:.2f}", low),
            fmt::format("{:.2f}", mid),
            fmt::format("{:.2f}", high)
        });
    }
    return tableData;
} 

std::string BollingerBands::toString() const {
    return fmt::format("Bollinger Bands: {}d, {}σ, {}", period, numStdDev, maType);
}