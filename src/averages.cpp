#include "averages.hpp"

std::map<std::time_t, double> simpleMovingAverage(const PriceSeries& ps, const double window) {
    const auto& data = ps.getData();
    if (data.size() < window) {
        throw std::invalid_argument("Cannot construct SMA: Window size is greater than data size");
    }
    
    std::map<std::time_t, double> sma;
    double sum(0.0);

    // Find initial window sum 
    auto w_end = data.begin();
    for (size_t i = 0; i < window; i++) {
        sum += w_end->second.getClose();
        w_end++;
    }

    // Slide window area until end date 
    auto w_start = data.begin();
    while (w_end != data.end()) {
        sma[w_end->first] = sum/window;
        sum += w_end->second.getClose() - (w_start)->second.getClose();
        w_end++;
        w_start++;
    }

    return sma;
}

std::map<std::time_t, double> exponentialMovingAverage(const PriceSeries& ps, const double window) {
    // TODO: ... 

    return {};
}