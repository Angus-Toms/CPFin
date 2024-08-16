#pragma once 

#ifndef MACD_HPP
#define MACD_HPP 

#include "overlays.hpp"
#include "ema.hpp" // Full include needed as getEMA called in calculate method

class PriceSeries;

class MACD : public IOverlay {
private:
    int aPeriod, bPeriod, cPeriod;
    TimeSeries<std::tuple<double, double, double>> data;

public:
    MACD(std::shared_ptr<PriceSeries> priceSeries, int aPeriod = 12, int bPeriod = 26, int cPeriod = 9);

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
    std::string toString() const override;
};

#endif // MACD_HPP