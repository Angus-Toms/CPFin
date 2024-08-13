#pragma once 

#ifndef RSI_HPP
#define RSI_HPP

#include "overlays.hpp"

class RSI : public IOverlay {
private:
    int period;
    std::map<std::time_t, double> data;

public:
    RSI(PriceSeries& priceSeries, int period = 14);

    void checkArguments() override;
    void calculate() override;
    void plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
    std::string toString() const;
};

#endif // RSI_HPP