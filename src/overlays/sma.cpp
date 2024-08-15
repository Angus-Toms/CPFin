#include "overlays/sma.hpp"
#include "priceseries.hpp"

SMA::SMA(std::shared_ptr<PriceSeries> priceSeries, int period) 
    : IOverlay(priceSeries), period(period) {

    // Set table printing values 
    name = fmt::format("SMA({}d)", period);
    columnHeaders = {"Date", "SMA"};
    columnWidths = {12, 10};

    checkArguments();
    calculate();
}

void SMA::checkArguments() {
}

void SMA::calculate() {
    const std::vector<std::time_t> dates = priceSeries->getDates();
    const std::vector<double> closes = priceSeries->getCloses();

    // Get first window 
    double sma = 0.0;
    for (int i = 0; i < period; ++i) {
        sma += closes[i];
    }
    sma /= period;

    // Slide window until end of data
    data[dates[period-1]] = sma;
    for (size_t i = period; i < closes.size(); ++i) {
        sma += (closes[i] - closes[i-period]) / period;
        data[dates[i]] = sma;
    }

}

void SMA::plot() const {
    namespace plt = matplotlibcpp;

    std::vector<std::time_t> xs;
    std::vector<double> ys;
    for (const auto& [date, sma] : data) {
        xs.push_back(date);
        ys.push_back(sma);
    }

    plt::named_plot(name, xs, ys, "-");
}

std::vector<std::vector<std::string>> SMA::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, sma] : data) {
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", sma)
        });
    }
    return tableData;
}

std::string SMA::toString() const {
    return "SMA";
}

const TimeSeries<double> SMA::getData() const {
    return data;
}