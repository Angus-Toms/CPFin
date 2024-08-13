#pragma once 

#ifndef EMA_HPP
#define EMA_HPP

#include "overlays.hpp"

class EMA : public IOverlay {
private:
    int period;
    double smoothingFactor;
    std::map<std::time_t, double> data;

public:
    EMA(PriceSeries& priceSeries, int period = 20, double smoothingFactor = -1);

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
    std::string toString() const;
};

#endif // EMA_HPP