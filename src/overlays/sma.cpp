#include "overlays/sma.hpp"
#include "priceseries.hpp"

SMA::SMA(PriceSeries& priceSeries, int period) 
    : period(period) {
    ps = std::make_unique<PriceSeries>(priceSeries);

    // Set table printing values 
    name = fmt::format("{}: SMA({}d)", ps.getTicker(), period);
    columnHeaders = {"Date", "SMA"};
    columnWidths = {12, 10};

    checkArguments();
    calculate();
}

void SMA::checkArguments() {
}

void SMA::calculate() {
    size_t periodSize = static_cast<size_t>(period);
    const std::map<std::time_t, OHCLRecord>& priceData = ps->getData();

    // Get first window sum 
    double sum = 0.0;
    auto wEnd = priceData.begin();
    for (size_t i = 0; i < periodSize; i++) {
        sum += wEnd->second.getClose();
        wEnd++;
    }

    wEnd--;
    data[wEnd->first] = sum / periodSize;

    // Slide window until end of data
    auto wStart = priceData.begin()--;
    while (++wEnd != priceData.end()) {
        sum += wEnd->second.getClose() - wStart->second.getClose();
        data[wEnd->first] = sum / periodSize;
        wStart++;
    }
}

void plot() const {
    namespace plt = matplotlibcpp;

    std::vector<std::time_t> xs;
    std::vector<double> ys;
    for (const auto& [date, sma] : data) {
        xs.push_back(date);
        ys.push_back(sma);
    }

    plt::named_plot(name, xs, ys, "r--");
}

std::vector<std::vector<std::string>> getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, sma] : data) {
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", sma)
        });
    }
    return tableData;
}

std::string toString() const {
    return getTable(getTableData(), columnHeaders, columnWidths);
}