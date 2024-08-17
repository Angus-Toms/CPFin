#pragma once 

#ifndef BOLLINGER_HPP
#define BOLLINGER_HPP

#include "ioverlay.hpp"
#include "types.hpp"

class PriceSeries;

class BollingerBands : public IOverlay {
private:
    int period;
    double numStdDev;
    MovingAverageType maType;
    std::map<std::time_t, std::tuple<double, double, double>> data;

public:
    BollingerBands(std::shared_ptr<PriceSeries> priceSeries, int period = 20, double numStdDev = 2, MovingAverageType maType = MovingAverageType::SMA);

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    TimeSeries<std::vector<double>> getDataMap() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

#endif // BOLLINGER_HPP