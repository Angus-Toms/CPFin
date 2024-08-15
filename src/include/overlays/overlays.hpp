#pragma once 

#ifndef OVERLAYS_HPP
#define OVERLAYS_HPP

#include <ctime>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "time_utils.hpp"
#include "print_utils.hpp"
#include "matplotlibcpp.h"

class PriceSeries;

class IOverlay {
protected:
    std::shared_ptr<PriceSeries> priceSeries;
    
    // Table printing values 
    std::string name;
    std::vector<std::string> columnHeaders;
    std::vector<int> columnWidths;

public:
    IOverlay() = default;
    virtual ~IOverlay() = default;

    // Constructor that takes ownership of a PriceSeries object
    IOverlay(std::shared_ptr<PriceSeries> priceSeries)
        : priceSeries(std::move(priceSeries)) {}

    virtual void checkArguments() {};
    virtual void calculate() {};
    virtual void plot() const {};
    virtual std::vector<std::vector<std::string>> getTableData() const { return {}; };
    virtual std::string toString() const { return ""; };
};

#endif // OVERLAYS_HPP