#include "averages.hpp"
#include "priceseries.hpp"

// SMA =========================================================================
// Constructor -----------------------------------------------------------------
SMA::SMA(const PriceSeries& priceSeries, int period)
    : priceSeries(priceSeries), period(period) {
    // Set table printing values 
    name = fmt::format("{}: SMA({}d)", priceSeries.getTicker(), period);
    columnHeaders = {"Date", "SMA"};
    columnWidths = {13, 10};

    calculate();
}
void SMA::calculate() {
    size_t periodSize = static_cast<size_t>(period);
    const std::map<std::time_t, OHCLRecord>& priceData = priceSeries.getData();
    // Check if window size is valid 
    if (periodSize > priceData.size()) {
        throw std::invalid_argument("Could not get SMA: Window size must be less than data length");
    }

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

// Virtual methods -------------------------------------------------------------
int SMA::plot() const {
    namespace plt = matplotlibcpp;

    std::vector<std::time_t> xs;
    std::vector<double> ys;
    for (const auto& [date, sma] : data) {
        xs.push_back(date);
        ys.push_back(sma);
    }

    plt::named_plot(name, xs, ys, "r--");
    
    return 0;
}
std::vector<std::vector<std::string>> SMA::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, sma] : data) {
        tableData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", sma)
        });
    }
    return tableData;
}

// EMA =========================================================================
// Constuctor ------------------------------------------------------------------
// Default period is 20, default smoothing factor is -1 (indicating needs to be calculated)
EMA::EMA(const PriceSeries& priceSeries, int period, double smoothingFactor)
    : priceSeries(priceSeries), period(period) {
    // Calculate smoothing factor if not specified
    if (smoothingFactor == -1) {
        smoothingFactor = 2.0 / (period + 1);
    }

    // Set table printing values 
    name = fmt::format("{}: EMA({}d, α={:.4f})", priceSeries.getTicker(), period, smoothingFactor);
    columnHeaders = {"Date", "EMA"};
    columnWidths = {13, 12};

    calculate();
}
void EMA::calculate() {
    const std::map<std::time_t, OHCLRecord>& priceData = priceSeries.getData();
    // Check if window size is valid
    if (static_cast<size_t>(period) > priceData.size()) {
        throw std::invalid_argument("Could not get EMA: Window size must be less than data length");
    }

    // Get simple sum from first window
    double sum(0);
    auto wStart = priceData.begin();
    auto idx = 0;
    while (idx < period) {
        sum += wStart->second.getClose();
        wStart++;
        idx++;
    }   

    // Calculate smoothing factor if not specified 
    if (!smoothingFactor) {
        smoothingFactor = 2.0 / (period + 1);
    }
    double ema = sum / period;

    // Move window until end of data 
    wStart--;
    while (wStart != priceData.end()) {
        data[wStart->first ] = ema; // ema = yesterdays price 
        wStart++;
        ema = (wStart->second.getClose() * smoothingFactor) + (ema * (1 - smoothingFactor));
    }
}

// Virtual methods -------------------------------------------------------------
int EMA::plot() const {
    std::cout << "TODO: Write EMA plot routine\n";
    return 0;
}

std::vector<std::vector<std::string>> EMA::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, ema] : data) {
        tableData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", ema)
        });
    }
    return tableData;
}

// MACD ========================================================================
// Constructor -----------------------------------------------------------------
MACD::MACD(const PriceSeries& priceSeries, int aPeriod, int bPeriod, int cPeriod)
    : priceSeries(priceSeries), aPeriod(aPeriod), bPeriod(bPeriod), cPeriod(cPeriod) {
    // Set table printing values 
    name = fmt::format("{}: MACD({}, {}, {})", priceSeries.getTicker(), aPeriod, bPeriod, cPeriod);
    columnHeaders = {"Date", "MACD", "Signal", "Divergence"};
    columnWidths = {13, 12, 12, 12};

    calculate();
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

// Virtual methods -------------------------------------------------------------
int MACD::plot() const {
    return 0;
}

std::vector<std::vector<std::string>> MACD::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, macdRecord] : data) {
        tableData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", macdRecord.macdLine),
            fmt::format("{:.2f}", macdRecord.signalLine),
            fmt::format("{:.2f}", macdRecord.histogram)
        });
    }
    return tableData;
}