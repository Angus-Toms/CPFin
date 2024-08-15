#pragma once 

#ifndef ENUMS_HPP
#define ENUMS_HPP

#include <chrono>
#include <map>

template <typename T>
using TimeSeries = std::map<std::time_t, T>;

enum class MovingAverageType {
    SMA,
    EMA
};

#endif // ENUMS_HPP