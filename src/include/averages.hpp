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
    SMA(const PriceSeries& priceSeries, int window = 20);

    // Implement virtual methods from TimeSeries
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

class EMA : public TimeSeries<double> {
private:
    const PriceSeries& priceSeries;
    int window;
    double smoothingFactor;

    void calculate();
public:
    EMA(const PriceSeries& priceSeries, int window = 20, double smoothingFactor = -1);

    // Implement virtual methods from TimeSeries 
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

#endif // AVERAGES_HPP