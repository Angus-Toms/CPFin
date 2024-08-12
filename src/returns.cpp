#include "returns.hpp"
#include "priceseries.hpp"

// Constructor -----------------------------------------------------------------
ReturnMetrics::ReturnMetrics(const PriceSeries& priceSeries)
    : priceSeries(priceSeries) {
    // Set table printing values
    name = fmt::format("{}: Returns", priceSeries.getTicker());
    columnHeaders = {"Date", "Daily", "Daily %", "Cumulative", "Annualized", "Log"};
    columnWidths = {13, 10, 10, 12, 12, 10};
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
        double daily = p - p_last;
        double dailyPct = (p - p_last) / p_last * 100;
        double cumulative = (p - p_start) / p_start * 100;
        double annualized = 0;
        double log = std::log(p / p_last);
        ReturnMetricRecord metrics(daily, dailyPct, cumulative, annualized, log);
        data[date] = metrics;

        // Update last price
        p_last = p;
    }
}

// Getters ---------------------------------------------------------------------
std::vector<double> ReturnMetrics::getDailys() const {
    std::vector<double> dailys;
    for (const auto& [date, record] : data) {
        dailys.push_back(record.getDaily());
    }
    return dailys;
}
std::vector<double> ReturnMetrics::getDailyPcts() const {
    std::vector<double> dailyPcts;
    for (const auto& [_, record] : data) {
        dailyPcts.push_back(record.getDailyPct());
    }
    return dailyPcts;
}
std::vector<double> ReturnMetrics::getCummulatives() const {
    std::vector<double> cummulatives;
    for (const auto& [_, record] : data) {
        cummulatives.push_back(record.getCumulative());
    }
    return cummulatives;
}
std::vector<double> ReturnMetrics::getAnnualizeds() const {
    std::vector<double> annualizeds;
    for (const auto& [_, record] : data) {
        annualizeds.push_back(record.getAnnualized());
    }
    return annualizeds;
}
std::vector<double> ReturnMetrics::getLogs() const {
    std::vector<double> logs;
    for (const auto& [_, record] : data) {
        logs.push_back(record.getLog());
    }
    return logs;
}

// Virtual methods -------------------------------------------------------------
int ReturnMetrics::plot() const {
    std::cout << "TODO: Write ReturnSeries plot routine\n";
    return 0;
}

std::vector<std::vector<std::string>> ReturnMetrics::getTableData() const {
    std::vector<std::vector<std::string>> allData;
    for (const auto& [date, record] : data) {
        allData.push_back({
            epochToDateString(date),
            fmt::format("{:.3f}", record.getDaily()),
            fmt::format("{:.3f}", record.getDailyPct()),
            fmt::format("{:.3f}", record.getCumulative()),
            fmt::format("{:.3f}", record.getAnnualized()),
            fmt::format("{:.4f}", record.getLog())
        });
    }
    return allData;
}