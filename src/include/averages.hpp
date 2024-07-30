#pragma once 

#ifndef AVERAGES_HPP 
#define AVERAGES_HPP

#include "timeseries.hpp"
#include "priceseries.hpp"

class SimpleMovingAverage : public TimeSeries<double> {
private:
    int window;

    // Private constructor 
    SimpleMovingAverage(const PriceSeries& priceSeries, int window)
        : window(window) {
        getSMA(priceSeries);
        }

    void getSMA(const PriceSeries& priceSeries);

public:
    // Virtual methods 
    ~SimpleMovingAverage() = default;
    int plot() const override;
    std::string toString() const override;

    // Factory methods 
    // Specified window size constructor 
    static SimpleMovingAverage getSimpleMovingAverage(const PriceSeries& priceSeries, int window);
    // Default window size constructor - defaults to 20d window
    static SimpleMovingAverage getSimpleMovingAverage(const PriceSeries& priceSeries);

};
#endif // AVERAGES_HPP