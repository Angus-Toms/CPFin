#include "overlays/bollinger.hpp"
#include "overlays/ema.hpp"
#include "overlays/sma.hpp"
#include "priceseries.hpp"

BollingerBands::BollingerBands(std::shared_ptr<PriceSeries> priceSeries, int period, double numStdDev, MovingAverageType maType) 
    : IOverlay(priceSeries), period(period), numStdDev(numStdDev), maType(maType) {

    // Set table printing values
    std::string maTypeString = maType == MovingAverageType::SMA ? "SMA" : "EMA";
    name = fmt::format("BB({}d, {}σ, {})", period, numStdDev, maTypeString);
    columnHeaders = {"Date", "Upper Band", "Middle Band", "Lower Band"};
    columnWidths = {12, 12, 12, 12};

    checkArguments();
    calculate();
}

void BollingerBands::checkArguments() {
    if (period < 1) {
        throw std::invalid_argument("Bollinger Bands period must be greater than 0");
    }
    if (period > priceSeries->getCount()) {
        throw std::invalid_argument("Bollinger Bands period must be less than the number of data points");
    }
    if (numStdDev <= 0) {
        throw std::invalid_argument("Bollinger Bands number of standard deviations must be greater than 0");
    }
}

void BollingerBands::calculate() {
    const auto& dates = priceSeries->getDates();
    const auto& closes = priceSeries->getCloses();

    const auto& mas = maType == MovingAverageType::SMA ?
        priceSeries->getSMA(period)->getData() :
        priceSeries->getEMA(period)->getData();

    // Get std dev of first window 
    double sums = 0.0;
    double squareSums = 0.0;
    for (int i = 0; i < period; ++i) {
        const auto close = closes[i];
        sums += close;
        squareSums += close * close;
    }

    for (size_t i = period; i < closes.size(); ++i) {
        // Get std dev 
        double stdDev = std::sqrt((squareSums - sums * sums / period) / period);
        const double ma = mas.at(dates[i]);
        data[dates[i]] = {
            ma - numStdDev * stdDev,
            ma,
            ma + numStdDev * stdDev
        };

        // Update StdDev 
        const double close_i = closes[i];
        const double close_i_period = closes[i-period];
        sums += close_i - close_i_period;
        squareSums += close_i * close_i - close_i_period * close_i_period;
    }
}

void BollingerBands::plot() const {
    namespace plt = matplotlibcpp;

    size_t n = data.size();
    std::vector<double> xs(n), lows(n), mids(n), highs(n);
    for (const auto& [date, val] : data) {
        xs.push_back(date);
        const auto& [low, mid, high] = val;
        lows.push_back(low);
        mids.push_back(mid);
        highs.push_back(high);
    }

    plt::named_plot("BB: Lower", xs, lows, ":");
    plt::named_plot("BB: Middle", xs, mids, ":");
    plt::named_plot("BB: Upper", xs, highs, ":");
}

TimeSeries<std::vector<double>> BollingerBands::getDataMap() const {
    TimeSeries<std::vector<double>> dataMap;
    for (const auto& [date, val] : data) {
        const auto& [low, mid, high] = val;
        dataMap[date] = {low, mid, high};
    }
    return dataMap;
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