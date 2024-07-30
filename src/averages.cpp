#include "averages.hpp"

void SimpleMovingAverage::getSMA(const PriceSeries& priceSeries) {
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

int SimpleMovingAverage::plot() const {
    std::cout << "TODO: Write SMA plot routine\n";
    return 0;
}

// Factory methods -------------------------------------------------------------
// Specified window size constructor
SimpleMovingAverage SimpleMovingAverage::getSimpleMovingAverage(const PriceSeries& priceSeries, int windowSize) {
    return SimpleMovingAverage(priceSeries, windowSize);
}
// Default window size constructor - defaults to 20d window
SimpleMovingAverage SimpleMovingAverage::getSimpleMovingAverage(const PriceSeries &priceSeries) {
    return SimpleMovingAverage(priceSeries, 20);
}

std::string SimpleMovingAverage::toString() const {
    std::ostringstream result;
    result << "| Simple Moving Average |\n-------------------------\n";
    for (auto& [date, sma] : data) {
        result << "| " << epochToDateString(date) << " | " << sma << "\n";
    }
    return result.str();
}