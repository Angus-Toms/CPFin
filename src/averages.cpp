#include "averages.hpp"
#include "priceseries.hpp"

// SMA =========================================================================
// Constructor -----------------------------------------------------------------
SMA::SMA(const PriceSeries& priceSeries, int window)
    : priceSeries(priceSeries), window(window) {
    // Set table printing values 
    name = fmt::format("{}: SMA({}d)", priceSeries.getTicker(), window);
    columnHeaders = {"Date", "SMA"};
    columnWidths = {13, 10};

    calculate();
}
void SMA::calculate() {
    size_t windowSize = static_cast<size_t>(window);
    const std::map<std::time_t, OHCLRecord>& priceData = priceSeries.getData();
    // Check if window size is valid 
    if (windowSize > priceData.size()) {
        throw std::invalid_argument("Could not get SMA: Window size must be less than data length");
    }

    // Get first window sum
    double sum = 0.0;
    auto wEnd = priceData.begin();
    for (size_t i = 0; i < windowSize; i++) {
        sum += wEnd->second.getClose();
        wEnd++;
    }

    wEnd--;
    data[wEnd->first] = sum / windowSize;

    // Slide window until end of data
    auto wStart = priceData.begin()--;
    while (++wEnd != priceData.end()) {
        sum += wEnd->second.getClose() - wStart->second.getClose();
        data[wEnd->first] = sum / windowSize;
        wStart++;
    }
}

// Virtual methods -------------------------------------------------------------
int SMA::plot() const {
    return 0;
}
std::vector<std::vector<std::string>> SMA::getTableData() const {
    std::vector<std::vector<std::string>> allData;
    for (const auto& [date, sma] : data) {
        allData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", sma)
        });
    }
    return allData;
}

// EMA =========================================================================
// Constuctor ------------------------------------------------------------------
// Default window is 20, default smoothing factor is -1 (indicating needs to be calculated)
EMA::EMA(const PriceSeries& priceSeries, int window, double smoothingFactor)
    : priceSeries(priceSeries), window(window) {
    // Calculate smoothing factor if not specified
    if (smoothingFactor == -1) {
        smoothingFactor = 2.0 / (window + 1);
    }

    // Set table printing values 
    name = fmt::format("{}: EMA({}d, α={:.4f})", priceSeries.getTicker(), window, smoothingFactor);
    columnHeaders = {"Date", "EMA"};
    columnWidths = {13, 12};

    calculate();
}
void EMA::calculate() {
    const std::map<std::time_t, OHCLRecord>& priceData = priceSeries.getData();
    // Check if window size is valid
    if (static_cast<size_t>(window) > priceData.size()) {
        throw std::invalid_argument("Could not get EMA: Window size must be less than data length");
    }

    // Get simple sum from first window
    double sum(0);
    auto wStart = priceData.begin();
    auto idx = 0;
    while (idx < window) {
        sum += wStart->second.getClose();
        wStart++;
        idx++;
    }   

    // Calculate smoothing factor if not specified 
    if (!smoothingFactor) {
        smoothingFactor = 2.0 / (window + 1);
    }
    double ema = sum / window;

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
    // aPeriod <= bPeriod, therefore there'll be more aPeriod data points than bPeriod 

    const SMA aSMA = priceSeries.getSMA(aPeriod);
    const SMA bSMA = priceSeries.getSMA(bPeriod);
    const std::map<std::time_t, double> aData = aSMA.getData();
    const std::map<std::time_t, double> bData = bSMA.getData();

    std::map<std::time_t, double> macd;
    for (const auto& [date, bSMAvalue] : bData) {
        macd[date] = aData.at(date) - bSMAvalue;
    }

    // Get c-day SMA of MACD line
    std::map<std::time_t, double> signal;
    // Get first window sum
    double sum = 0.0;
    auto wEnd = macd.begin();
    for (size_t i = 0; i < static_cast<size_t>(cPeriod); i++) {
        sum += wEnd->second;
        wEnd++;
    }

    wEnd--;
    signal[wEnd->first] = sum / cPeriod;

    // Slide window until end of data 
    auto wStart = macd.begin()--;
    while (++wEnd != macd.end()) {
        sum += wEnd->second - wStart->second;
        signal[wEnd->first] = sum / cPeriod;
        wStart++;
    } 


    // Get histogram, construct MACDRecords
    for (const auto& [date, signalValue] : signal) {
        auto macdValue = macd.at(date);
        data[date] = MACDRecord(
            macdValue,
            signalValue,
            macdValue - signalValue
        );
    }
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