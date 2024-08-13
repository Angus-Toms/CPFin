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

class PriceSeries;

class IOverlay {
private:
    const std::unique_ptr<PriceSeries> ps;
    
    // Table printing values 
    std::string name;
    std::vector<std::string> columnHeaders;
    std::vector<int> columnWidths;

public:
    virtual ~IOverlay() = default;

    virtual void checkArguments() = 0;
    virtual void calculate() = 0;
    virtual void plot() const = 0;
    virtual std::vector<std::vector<std::string>> getTableData() const = 0;
    virtual std::string toString() const;
};

#endif // OVERLAYS_HPP