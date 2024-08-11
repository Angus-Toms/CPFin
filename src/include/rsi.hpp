#pragma once 
#ifndef RSI_HPP 
#define RSI_HPP 

#include "timeseries.hpp"

class PriceSeries;

class RSI : public TimeSeries<double> {
private:
    const PriceSeries& priceSeries;
    int period;

    void calculate();

public:
    RSI(const PriceSeries& priceSeries, int period = 14);

    // Implement virtual methods from TimeSeries 
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

#endif // RSI_HPP