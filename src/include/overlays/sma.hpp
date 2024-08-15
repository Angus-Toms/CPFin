#pragma once 

#ifndef SMA_HPP
#define SMA_HPP

#include "overlays.hpp"

class PriceSeries;

class SMA : public IOverlay {
private:
    int period;
    std::map<std::time_t, double> data;

public:
    SMA(std::shared_ptr<PriceSeries> priceSeries, int period = 20);

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
    std::string toString() const override;
};

#endif // SMA_HPP