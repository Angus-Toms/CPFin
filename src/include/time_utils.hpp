#pragma once 

#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

constexpr std::time_t DAY_DURATION = 60 * 60 * 24;
constexpr std::time_t WEEK_DURATION = DAY_DURATION * 7;
constexpr std::time_t MONTH_DURATION = DAY_DURATION * 30;
constexpr std::time_t YEAR_DURATION = DAY_DURATION * 365;

std::time_t dateStringToEpoch(const std::string& dateStr);
std::string epochToDateString(std::time_t time);

#endif // TIME_UTILS_HPP