#include "overlays/rsi.hpp"
#include "priceseries.hpp"

RSI::RSI(std::shared_ptr<PriceSeries> priceSeries, int period) 
    : IOverlay(std::move(priceSeries)), period(period) {

    // Set table printing values
    name = fmt::format("{}: RSI({}d)", priceSeries->getTicker(), period);
    columnHeaders = {"Date", "RSI"};
    columnWidths = {12, 10};

    checkArguments();
    calculate();
}

void RSI::checkArguments() {
}

void RSI::calculate() {
    // const std::vector<double> returns = priceSeries.getReturns()->getDailys();
    // std::vector<std::time_t> dates = priceSeries.getDates;
    // double avgGain = 0, avgLoss = 0;

    // for (int i = 1; i <= period; ++i) {
    //     double r = returns[1];
    //     if (r < 0) {
    //         avgLoss -= r;
    //     } else if (r > 0) {
    //         avgGain += r;
    //     }
    // }
    // avgLoss /= period;
    // avgGain /= period;

    // // Slide window and calculate RSI 
    // for (size_t i = period+1; i <= returns.size(); ++i) {
    //     double rs = avgGain / avgLoss;
    //     double rsi = 100 - (100 / (1 + rs));
    //     data[dates[i-1]] = rsi;

    //     // Update gains and losses
    //     double r = returns[i];
    //     double gain = r > 0 ? r : 0;
    //     double loss = r < 0 ? -r : 0;

    //     avgGain = ((avgGain * (period-1) + gain)) / period;
    //     avgLoss = ((avgLoss * (period-1) + loss)) / period;
    // }
}

void RSI::plot() const {
    namespace plt = matplotlibcpp;

    std::vector<std::time_t> xs;
    std::vector<double> ys;
    for (const auto& [date, rsi] : data) {
        xs.push_back(date);
        ys.push_back(rsi);
    }

    plt::named_plot(name, xs, ys, "o--");
}

std::vector<std::vector<std::string>> RSI::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, rsi] : data) {
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", rsi)
        });
    }
    return tableData;
}

std::string RSI::toString() const {
    return "RSI";
}