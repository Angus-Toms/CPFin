#pragma once 

#ifndef TIMESERIES_HPP
#define TIMESERIES_HPP

#include <map>
#include <ctime>
#include <vector>

template <typename T>
class TimeSeries {
protected:
    std::map<std::time_t, T> data;

    // Pure virtual method, each subclass has different implementation
    // based on the data they store
    // virtual std::vector<std::vector<double>> getAllData() const = 0;

public:
    virtual ~TimeSeries() = default;
    virtual int plot() const = 0;
    virtual std::string toString() const = 0;
};

#endif // TIMESERIES_HPP