#include "overlays/macd.hpp"
#include "priceseries.hpp"

MACD::MACD(std::shared_ptr<PriceSeries> priceSeries, int aPeriod, int bPeriod, int cPeriod)
    : IOverlay(std::move(priceSeries)), aPeriod(aPeriod), bPeriod(bPeriod), cPeriod(cPeriod) {

    // Set table printing values
    name = fmt::format("{}: MACD({}, {}, {})", priceSeries->getTicker(), aPeriod, bPeriod, cPeriod);
    columnHeaders = {"Date", "MACD", "Signal", "Divergence"};
    columnWidths = {13, 12, 12, 12};

    checkArguments();
    calculate();
}

void MACD::checkArguments() {
}

void MACD::calculate() {
    // TODO: Lots of argument checking needed for this routine 
    // TODO: Change highlighting needs to be redone for this analysis - check Wikipedia page for how this is interpreted
    
    // Get MACD line
    // const std::map<std::time_t, double> aData = priceSeries.getSMA(aPeriod)->getData();
    // const std::map<std::time_t, double> bData = priceSeries.getSMA(bPeriod)->getData();

    // std::map<std::time_t, double> macd;
    // for (const auto& [date, bSMAvalue] : bData) {
    //     macd[date] = aData.at(date) - bSMAvalue;
    // }

    // // Get c-day SMA of MACD line
    // std::map<std::time_t, double> signal;
    // // Get first window sum
    // double sum = 0.0;
    // auto wEnd = macd.begin();
    // for (size_t i = 0; i < static_cast<size_t>(cPeriod); i++) {
    //     sum += wEnd->second;
    //     wEnd++;
    // }

    // wEnd--;
    // signal[wEnd->first] = sum / cPeriod;

    // // Slide window until end of data 
    // auto wStart = macd.begin()--;
    // while (++wEnd != macd.end()) {
    //     sum += wEnd->second - wStart->second;
    //     signal[wEnd->first] = sum / cPeriod;
    //     wStart++;
    // } 


    // // Get histogram, construct MACDRecords
    // for (const auto& [date, signalValue] : signal) {
    //     auto macdValue = macd.at(date);
    //     data[date] = MACDRecord(
    //         macdValue,
    //         signalValue,
    //         macdValue - signalValue
    //     );
    // }
}

void MACD::plot() const {

}

std::vector<std::vector<std::string>> MACD::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, val] : data) {
        const auto& [macd, signal, divergence] = val;
        tableData.push_back({
            fmt::format(epochToDateString(date)),
            fmt::format("{:.2f}", macd),
            fmt::format("{:.2f}", signal),
            fmt::format("{:.2f}", divergence)
        });
    }
    return tableData;
}

std::string MACD::toString() const {
    return "MACD";
}