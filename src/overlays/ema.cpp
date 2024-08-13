#include "overlays/ema.hpp"

EMA::EMA(PriceSeries& priceSeries, int period, double smoothingFactor)
    : period(period), smoothingFactor(smoothingFactor) {
    ps = std::make_unique<PriceSeries>(priceSeries);

    // Calculate smoothing factor if not specified
    if (smoothingFactor == -1) {
        smoothingFactor = 2.0 / (period+1);
    }

    // Set table printing values
    name = fmt::format("{}: EMA({}d, α={:.2f})", ps->getTicker(), period, smoothingFactor);
    columnHeaders = {"Date", "EMA"};
    columnWidths = {12, 10};

    checkArguments();
    calculate();
}

void EMA::checkArguments() {

}

void EMA::calculate() {
    const std::map<std::time_t, OHCLRecord>& priceData = ps.getData();

    double ema = 0.0;
    auto wStart = priceData.begin();
    auto idx = 0;
    while (idx < period) {
        ema += wStart->second.getClose();
        wStart++;
        idx++;
    }

    ema /= period;

    // Slide window until end of data
    wStart--;
    while (wStart != priceData.end()) {
        data[wStart->first] = ema
        wStart++;
        ema = (wStart->second.getClose() * smoothingFactor) + (ema * (1 - smoothingFactor));
    }
}

void EMA::plot() const {
    namespace plt = matplotlibcpp;

    std::vector<std::time_t> xs;
    std::vector<double> ys;
    for (const auto& [date, ema] : data) {
        xs.push_back(date);
        ys.push_back(ema);
    }

    plt::named_plot(name, xs, ys, "r--");   
}

std::vector<std::vector<std::string>> getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, ema] : data) {
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", ema)
        });
    }
    return tableData;
}

std::string toString() const {
    return name;
}