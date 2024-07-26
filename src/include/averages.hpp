#pragma once 

#ifndef AVERAGES_HPP 
#define AVERAGES_HPP

#include <map>
#include <ctime>

#include "series.hpp"

std::map<std::time_t, double> simpleMovingAverage(const PriceSeries& ps, const double window);
std::map<std::time_t, double> exponentialMovingAverage(const PriceSeries& ps, const double window);

#endif // AVERAGES_HPP