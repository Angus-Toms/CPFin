#include "overlays/macd.hpp"
#include "priceseries.hpp"

MACD::MACD(std::shared_ptr<PriceSeries> priceSeries, int aPeriod, int bPeriod, int cPeriod)
    : IOverlay(priceSeries), aPeriod(aPeriod), bPeriod(bPeriod), cPeriod(cPeriod) {

    // Set table printing values
    name = fmt::format("MACD({}, {}, {})", aPeriod, bPeriod, cPeriod);
    columnHeaders = {"Date", "MACD", "Signal", "Divergence"};
    columnWidths = {13, 12, 12, 12};

    checkArguments();
    calculate();
}

void MACD::checkArguments() {
    if (aPeriod < 1 || bPeriod < 1 || cPeriod < 1) {
        throw std::invalid_argument("Could not construct MACD: periods must be greater than 0");
    }
    int count = priceSeries->getCount();
    if (aPeriod > count|| bPeriod > count|| cPeriod > count) {
        throw std::invalid_argument("Could not construct MACD: periods must be less than the number of data points");
    }
}

void MACD::calculate() {
    // Get MACD line
    const TimeSeries<double> aData = priceSeries->getEMA(aPeriod)->getData();
    const TimeSeries<double> bData = priceSeries->getEMA(bPeriod)->getData();

    TimeSeries<double> macd; // MACD = EMA_a - EMA_b
    for (const auto& [date, bEMAvalue] : bData) {
        if (aData.count(date)) {
            macd[date] = aData.at(date) - bEMAvalue;
        }
    }

    // Signal line = EMA_c(MACD)
    // Get first period sum
    TimeSeries<double> signal;
    double multiplier = 2.0 / (cPeriod + 1);
    double sum = 0.0;
    auto it = macd.begin();
    for (int i = 0; i < cPeriod; ++i, ++it) {
        sum += it->second;
    }
    signal[it->first] = sum / cPeriod;

    // Update window
    for (++it; it != macd.end(); ++it) {
        double value = (it->second - signal.rbegin()->second) * multiplier + signal.rbegin()->second;
        signal[it->first] = value;
    }

    // Construct MACD data
    for (const auto& [date, signalValue] : signal) {
        double macdValue = macd.at(date);
        data[date] = std::make_tuple(
            macdValue,
            signalValue,
            macdValue - signalValue
        );
    }
}


void MACD::plot() const {
    // This needs to be a subplot
    namespace plt = matplotlibcpp;
    std::vector<std::time_t> xs;
    std::vector<double> macd, signal, divergence;

    for (const auto& [date, val] : data) {
        const auto& [macdVal, signalVal, divergenceVal] = val;
        xs.push_back(date);
        macd.push_back(macdVal);
        signal.push_back(signalVal);
        divergence.push_back(divergenceVal);
    }

    plt::named_plot("MACD", xs, macd, "-");
    plt::named_plot("Signal", xs, signal, "-");
    plt::bar(xs, divergence, {}, intervalToSeconds("1d") * 0.8, 0, {"grey"});
    plt::legend();
    plt::xlim(xs.front() - intervalToSeconds("1d"), xs.back() + intervalToSeconds("1d"));
}

TimeSeries<std::vector<double>> MACD::getDataMap() const {
    TimeSeries<std::vector<double>> dataMap;
    for (const auto& [date, val] : data) {
        const auto& [macd, signal, divergence] = val;
        dataMap[date] = {macd, signal, divergence};
    }
    return dataMap;
}

std::vector<std::vector<std::string>> MACD::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, val] : data) {
        const auto& [macd, signal, divergence] = val;
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", macd),
            fmt::format("{:.2f}", signal),
            fmt::format("{:.2f}", divergence)
        });
    }
    return tableData;
}