#pragma once 

#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <string>
#include <array>
#include <algorithm>

constexpr std::time_t MINUTE_DURATION = 60;
constexpr std::time_t HOUR_DURATION = MINUTE_DURATION * 60;
constexpr std::time_t DAY_DURATION = HOUR_DURATION * 24;
constexpr std::time_t WEEK_DURATION = DAY_DURATION * 7;
constexpr std::time_t MONTH_DURATION = DAY_DURATION * 30;
constexpr std::time_t YEAR_DURATION = DAY_DURATION * 365;

constexpr std::array<std::string_view, 6> VALID_INTERVALS{"1m", "1h", "1d", "1wk", "1mo", "1y"};

std::time_t dateStringToEpoch(const std::string& dateStr);
std::string epochToDateString(const std::time_t date, bool includeTime = false);
std::time_t intervalToSeconds(const std::string& interval);
bool isInvalidInterval(const std::string& interval);

#endif // TIME_UTILS_HPP