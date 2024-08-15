#pragma once 

#ifndef EMA_HPP
#define EMA_HPP

#include "overlays.hpp"

class PriceSeries;

class EMA : public IOverlay {
private:
    int period;
    double smoothingFactor;
    TimeSeries<double> data;

public:
    EMA(std::shared_ptr<PriceSeries> priceSeries, int period = 20, double smoothingFactor = -1);

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
    std::string toString() const override;

    const TimeSeries<double> getData() const;
};

#endif // EMA_HPP