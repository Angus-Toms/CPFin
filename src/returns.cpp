#include "returns.hpp"

// ReturnMetrics struct ========================================================
std::string ReturnMetrics::toString() const {
    return "TODO: Write ReturnMetrics toString routine";
};

// ReturnSeries class ==========================================================
void ReturnSeries::getReturns(const PriceSeries& priceSeries) {
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
        ReturnMetrics metrics(daily, cumulative, annualized, log);
        data[date] = metrics;

        // Update last price
        p_last = p;
    }
 }

// Factory method --------------------------------------------------------------
ReturnSeries ReturnSeries::getReturnSeries(const PriceSeries& priceSeries) {
    return ReturnSeries(priceSeries);
}

// Virtual methods ------------------------------------------------------------
int ReturnSeries::plot() const {
    std::cout << "TODO: Write ReturnSeries plot routine\n";
    return 0;
}

std::string ReturnSeries::toString() const {
    // Table constants 
    std::vector<int> columnWidths = {13, 10, 10, 10, 10};
    std::vector<Justification> justifications = {Justification::LEFT, 
        Justification::RIGHT, Justification::RIGHT, Justification::RIGHT, 
        Justification::RIGHT};
    std::vector<Color> colors = {Color::WHITE, Color::WHITE, Color::WHITE, 
        Color::WHITE, Color::WHITE};
    int totalWidth = 57;

    // Table title 
    std::string str = getTopLine({totalWidth});
    str += getRow({fmt::format("{} - Returns", ticker)}, {totalWidth}, 
        {Justification::CENTER}, {Color::WHITE});
    
    // Table headers 
    str += getMidLine(columnWidths, Ticks::LOWER);
    str += getRow({"Date", "Daily", "Cumulative", "Annualised", "Log"}, columnWidths, justifications, colors);

    // Table data 
    str += getMidLine(columnWidths, Ticks::BOTH);
    double lastDaily, lastCumulative, lastAnnualized, lastLog;
    bool isFirstRow = true;
    for (const auto& [date, metrics] : data) {
        double daily = metrics.getDaily();
        double cumulative = metrics.getCumulative();
        double annualized = metrics.getAnnualized();
        double log = metrics.getLog();

        std::vector<std::string> row = {
            epochToDateString(date),
            fmt::format("{:.2f}", daily),
            fmt::format("{:.2f}", cumulative),
            fmt::format("{:.2f}", annualized),
            fmt::format("{:.4f}", log)
        };

        // Color changes
        if (!isFirstRow) {
            if (daily > lastDaily) {
                colors[1] = Color::GREEN;
            } else if (daily < lastDaily) {
                colors[1] = Color::RED;
            } else {
                colors[1] = Color::WHITE;
            }

            if (cumulative > lastCumulative) {
                colors[2] = Color::GREEN;
            } else if (cumulative < lastCumulative) {
                colors[2] = Color::RED;
            } else {
                colors[2] = Color::WHITE;
            }

            if (annualized > lastAnnualized) {
                colors[3] = Color::GREEN;
            } else if (annualized < lastAnnualized) {
                colors[3] = Color::RED;
            } else {
                colors[3] = Color::WHITE;
            }

            if (log > lastLog) {
                colors[4] = Color::GREEN;
            } else if (log < lastLog) {
                colors[4] = Color::RED;
            } else {
                colors[4] = Color::WHITE;
            }

        } else {
            isFirstRow = false;
        }

        lastDaily = daily;
        lastCumulative = cumulative;
        lastAnnualized = annualized;
        lastLog = log;
        str += getRow(row, columnWidths, justifications, colors);
    }
    str += getBottomLine(columnWidths);
    return str;
}


