#pragma once 

#ifndef TIMESERIES_HPP
#define TIMESERIES_HPP

#include <map>
#include <ctime>

template <typename T>
class TimeSeries {
protected:
    std::map<std::time_t, T> data;

public:
    virtual ~TimeSeries() = default;

    // Pure virtual methods
    virtual int plot() const = 0;
    virtual std::string toString() const = 0;
};

#endif // TIMESERIES_HPP