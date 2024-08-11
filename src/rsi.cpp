#include "rsi.hpp"
#include "returns.hpp"
#include "priceseries.hpp"

// RSI =========================================================================
// Constructor -----------------------------------------------------------------
RSI::RSI(const PriceSeries& priceSeries, int period)
    : priceSeries(priceSeries), period(period) {
    // Set table printing values 
    name = fmt::format("{}: RSI({}d)", priceSeries.getTicker(), period);
    columnHeaders = {"Date", "RSI"};
    columnWidths = {13, 10};

    calculate();
}
void RSI::calculate() {
    // Get daily changes 
    const std::vector<double> returns = priceSeries.getReturns().getDailys();

    // Mark gains and losses in first window;
    double avgGain = 0;
    double avgLoss = 0;

    for (int i = 0; i < period; i++) {
        double r = returns[i+1]; // Ignore first day
        if (r < 0) {
            avgLoss -= r;
        } else if (r > 0) {
            avgGain += r;
        }
    }

    // Calculate average gains and losses for initial window
    avgGain /= period;
    avgLoss /= period;

    // Slide window
    std::vector<std::time_t> dates = priceSeries.getDates();
    for (size_t i = period+1; i <= returns.size(); i++) {
        // Update sums 
        double r = returns[i];
        if (r < 0) {
            avgLoss = ((avgLoss * period-1) - r) / period;
        } else {
            avgGain = ((avgGain * period-1) + r) / period;
        }

        double rs = avgGain / avgLoss;
        double rsi = 100 - 100 / (1+rs);
        data[dates[i-1]] = rsi;
    }
}

// Virtual methods -------------------------------------------------------------
int RSI::plot() const {
    return 0;
}
std::vector<std::vector<std::string>> RSI::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, rsi] : data) {
        tableData.push_back({
            epochToDateString(date),
            fmt::format("{:.3f}", rsi)
        });
    }
    return tableData;
}