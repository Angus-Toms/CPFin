#pragma once 

#ifndef BOLLINGER_HPP
#define BOLLINGER_HPP

#include "overlays.hpp"

class PriceSeries;

class BollingerBands : public IOverlay {
private:
    int period;
    double numStdDev;
    std::string maType;
    std::map<std::time_t, std::tuple<double, double, double>> data;

public:
    BollingerBands(std::shared_ptr<PriceSeries> priceSeries, int period = 20, double numStdDev = 2, const std::string& maType = "sma");

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
    std::string toString() const override;
};

#endif // BOLLINGER_HPP