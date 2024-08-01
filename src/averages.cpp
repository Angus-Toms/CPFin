#include "averages.hpp"
#include "priceseries.hpp"

// Constructor -----------------------------------------------------------------
SMA::SMA(const PriceSeries& priceSeries, int window)
    : priceSeries(priceSeries), window(window) {
    // Set table printing values 
    name = fmt::format("{} - SMA ({}d)", priceSeries.getTicker(), window);
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
    double sum(0.0);
    auto w_end = priceData.begin();
    for (size_t i=0; i<windowSize; ++i) {
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
    std::cout << "TODO: Write SMA plot routine\n";
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