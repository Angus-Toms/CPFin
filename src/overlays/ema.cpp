#include "overlays/ema.hpp"
#include "priceseries.hpp"

EMA::EMA(std::shared_ptr<PriceSeries> priceSeries, int period, double smoothingFactor)
    : IOverlay(priceSeries), period(period), smoothingFactor(smoothingFactor) {

    // Calculate smoothing factor if not specified
    if (this->smoothingFactor == -1) {
        this->smoothingFactor = 2.0 / (period+1);
    }

    // Set table printing values
    name = fmt::format("EMA({}d, α={:.2f})", period, this->smoothingFactor);
    columnHeaders = {"Date", "EMA"};
    columnWidths = {12, 10};

    checkArguments();
    calculate();
}

void EMA::checkArguments() {

}

void EMA::calculate() {
    const std::vector<std::time_t>& dates = priceSeries->getDates();
    const std::vector<double> closes = priceSeries->getCloses();

    // Calculate SMA of first window
    double ema = 0.0;
    for (int i = 0; i < period; i++) {
        ema += closes[i];
    }
    ema /= period;

    // Slide window until end of data
    data[dates[period-1]] = ema;
    for (size_t i = period; i < closes.size(); i++) {
        ema = (closes[i] * smoothingFactor) + (ema * (1 - smoothingFactor));
        data[dates[i]] = ema;
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

    plt::named_plot(name, xs, ys, "--");   
}

std::vector<std::vector<std::string>> EMA::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, ema] : data) {
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", ema)
        });
    }
    return tableData;
}

const TimeSeries<double> EMA::getData() const {
    return data;
}