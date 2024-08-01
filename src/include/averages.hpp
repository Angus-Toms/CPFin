#pragma once 

#ifndef AVERAGES_HPP 
#define AVERAGES_HPP

#include "timeseries.hpp"

class PriceSeries;

class SMA : public TimeSeries<double> {
private:
    const PriceSeries& priceSeries;
    int window;

    void calculate();

public:
    SMA(const PriceSeries& priceSeries, int window);

    // Implement virtual methods from TimeSeries
    int plot() const override;
    std::string toString() const override;

};
#endif // AVERAGES_HPP