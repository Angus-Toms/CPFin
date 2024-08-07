#pragma once 

#ifndef BOLLINGER_HPP
#define BOLLINGER_HPP

#include "enums.hpp"
#include "timeseries.hpp"

#include <fmt/format.h>
#include <matplot/matplot.h>

class PriceSeries;

class BollingerBands : public TimeSeries<std::tuple<double, double, double>> {
private:
    const PriceSeries& priceSeries;
    int window;
    double numStdDev;
    MovingAverageType maType;

    void calculate();

public:
    BollingerBands(const PriceSeries& priceSeries, int window = 20, double numStdDev = 2, MovingAverageType maType = MovingAverageType::SMA);

    // Implement virtual methods from TimeSeries 
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

#endif // BOLLINGER_HPP