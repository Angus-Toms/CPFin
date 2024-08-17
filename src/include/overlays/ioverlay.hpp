#pragma once 

#ifndef IOVERLAY_HPP
#define IOVERLAY_HPP

#include <ctime>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "time_utils.hpp"
#include "types.hpp"
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

    virtual void checkArguments() = 0;
    virtual void calculate() = 0;
    virtual void plot() const = 0;
    // Get map where each row of data is a vector
    // used for printing price series with overlays
    virtual TimeSeries<std::vector<double>> getDataMap() const = 0;
    virtual std::vector<std::vector<std::string>> getTableData() const = 0;

    const std::string getName() const { return name; }
    const std::vector<std::string> getColumnHeaders() const { return columnHeaders; }
    const std::vector<int> getColumnWidths() const { return columnWidths; }
    const std::string toString() const {
        return getTable(
            name,
            getTableData(),
            columnWidths,
            columnHeaders,
            true
        );
    }
};

#endif // IOVERLAY_HPP