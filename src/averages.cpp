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
    double sum(0);
    auto w_end = priceData.begin();
    for (size_t i = 0; i < windowSize; i++) {
        sum += w_end->second.getClose();
        w_end++;
    }

    w_end--;
    data[w_end->first] = sum / windowSize;

    // Slide window until end of data
    auto w_start = priceData.begin()--;
    while (++w_end != priceData.end()) {
        sum += w_end->second.getClose() - w_start->second.getClose();
        data[w_end->first] = sum / windowSize;
        w_start++;
    }
}


// Virtual methods -------------------------------------------------------------
int SMA::plot() const {

    return 0;
}

std::vector<std::vector<std::string>> SMA::getAllData() const {
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
    auto w_start = priceData.begin();
    auto idx = 0;
    while (idx < window) {
        sum += w_start->second.getClose();
        w_start++;
        idx++;
    }   

    // Calculate smoothing factor if not specified 
    if (!smoothingFactor) {
        smoothingFactor = 2.0 / (window + 1);
    }
    double ema = sum / window;

    // Move window until end of data 
    w_start--;
    while (w_start != priceData.end()) {
        data[w_start->first ] = ema; // ema = yesterdays price 
        w_start++;
        ema = (w_start->second.getClose() * smoothingFactor) + (ema * (1 - smoothingFactor));
    }
    
    
}

// Virtual methods -------------------------------------------------------------
int EMA::plot() const {
    std::cout << "TODO: Write EMA plot routine\n";
    return 0;
}

std::vector<std::vector<std::string>> EMA::getAllData() const {
    std::vector<std::vector<std::string>> allData;
    for (const auto& [date, ema] : data) {
        allData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", ema)
        });
    }
    return allData;
}