#include "overlays/rsi.hpp"
#include "priceseries.hpp"

RSI::RSI(std::shared_ptr<PriceSeries> priceSeries, int period) 
    : IOverlay(priceSeries), period(period) {

    // Set table printing values
    name = fmt::format("RSI({}d)", period);
    columnHeaders = {"Date", "RSI"};
    columnWidths = {12, 10};

    checkArguments();
    calculate();
}

void RSI::checkArguments() {
}

void RSI::calculate() {
    // Get day-to-day returns
    std::vector<std::time_t> dates = priceSeries->getDates();
    std::vector<double> closes = priceSeries->getCloses();
    std::vector<double> returns(closes.size() - 1);
    for (size_t i = 1; i < closes.size(); ++i) {
        returns[i - 1] = closes[i] - closes[i - 1];
    }

    double avgGain = 0, avgLoss = 0;
    for (int i = 0; i < period; ++i) {
        double r = returns[i];
        if (r < 0) {
            avgLoss -= r;
        } else {
            avgGain += r;
        }
    }
    avgLoss /= period;
    avgGain /= period;

    // Slide window and calculate RSI 
    for (size_t i = period; i < returns.size(); ++i) {
        double rs = avgGain / avgLoss;
        double rsi = 100 - (100 / (1 + rs));
        data[dates[i]] = rsi;

        // Update gains and losses
        double r = returns[i];
        double gain = r > 0 ? r : 0;
        double loss = r < 0 ? -r : 0;

        avgGain = ((avgGain * (period - 1)) + gain) / period;
        avgLoss = ((avgLoss * (period - 1)) + loss) / period;
    }
}

void RSI::plot() const {
    namespace plt = matplotlibcpp;
    std::vector<std::time_t> xs;
    std::vector<double> ys;
    for (const auto& [date, rsi] : data) {
        xs.push_back(date);
        ys.push_back(rsi);
    }

    plt::plot(xs, ys, "-");
    std::map<std::string, std::string> kwargs;
    kwargs["color"] = "red";
    kwargs["linestyle"] = "--";
    plt::axhline(70, 0., 1., kwargs);
    plt::axhline(30, 0., 1., kwargs);
    plt::xlim(xs.front() - intervalToSeconds("1d"), xs.back() + intervalToSeconds("1d"));
    plt::ylabel("RSI");
    plt::ylim(0, 100);
}

TimeSeries<std::vector<double>> RSI::getDataMap() const {
    TimeSeries<std::vector<double>> dataMap;
    for (const auto& [date, rsi] : data) {
        dataMap[date] = {rsi};
    }
    return dataMap;
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