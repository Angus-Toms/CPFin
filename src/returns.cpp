#include "returns.hpp"
#include "priceseries.hpp"

// Constructor -----------------------------------------------------------------
ReturnMetrics::ReturnMetrics(const PriceSeries& priceSeries)
    : priceSeries(priceSeries) {
    // Set table printing values
    name = fmt::format("{} - Returns", priceSeries.getTicker());
    columnHeaders = {"Date", "Daily", "Cumulative", "Annualized", "Log"};
    columnWidths = {13, 10, 12, 12, 10};
    calculate();
}

void ReturnMetrics::calculate() {
    const std::map<std::time_t, OHCLRecord>& priceData = priceSeries.getData();

    // Get first price record
    double p_start = priceData.begin()->second.getClose();
    double p_last = p_start;
    
    for (const auto& [date, record] : priceData) {
        double p = record.getClose();

        // Get metrics and add
        double daily = (p - p_last) / p_last * 100;
        double cumulative = (p - p_start) / p_start * 100;
        double annualized = 0;
        double log = std::log(p / p_last);
        ReturnMetricRecord metrics(daily, cumulative, annualized, log);
        data[date] = metrics;

        // Update last price
        p_last = p;
    }
}

// Virtual methods -------------------------------------------------------------
int ReturnMetrics::plot() const {
    std::cout << "TODO: Write ReturnSeries plot routine\n";
    return 0;
}

std::vector<std::vector<std::string>> ReturnMetrics::getAllData() const {
    std::vector<std::vector<std::string>> allData;
    for (const auto& [date, record] : data) {
        allData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", record.getDaily()),
            fmt::format("{:.2f}", record.getCumulative()),
            fmt::format("{:.2f}", record.getAnnualized()),
            fmt::format("{:.2f}", record.getLog())
        });
    }
    return allData;
}
