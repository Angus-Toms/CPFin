#include "overlays/bollinger.hpp"
#include "priceseries.hpp"

BollingerBands::BollingerBands(std::shared_ptr<PriceSeries> priceSeries, int period, double numStdDev, const std::string& maType) 
    : IOverlay(std::move(priceSeries)), period(period), numStdDev(numStdDev), maType(maType) {

    // Set table printing values
    name = fmt::format("{}: BB({}d, {}σ, {})", priceSeries->getTicker(), period, numStdDev, maType);
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
    std::vector<double> highs;
    for (const auto& [date, val] : data) {
        const auto& [low, mid, high] = val;
        lows.push_back(low);
        mids.push_back(mid);
        highs.push_back(high);
    }

    plt::named_plot("Low", xs, lows, "r-");
    plt::named_plot("Middle", xs, mids, "g");
    plt::named_plot("Upper", xs, highs, "r-");
}

std::vector<std::vector<std::string>> BollingerBands::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, val] : data) {
        const auto& [low, mid, high] = val;
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