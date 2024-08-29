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

constexpr std::time_t intervalToSeconds(const std::string& interval) {
    if (interval == "1m") {
        return MINUTE_DURATION;
    } else if (interval == "1h") {
        return HOUR_DURATION;
    } else if (interval == "1d") {
        return DAY_DURATION;
    } else if (interval == "1wk") {
        return WEEK_DURATION;
    } else if (interval == "1mo") {
        return MONTH_DURATION;
    } else if (interval == "1y") {
        return YEAR_DURATION;
    } else {
        return -1;
    }
}
constexpr bool isInvalidInterval(const std::string& interval) {
    return std::find(VALID_INTERVALS.begin(), VALID_INTERVALS.end(), interval) == VALID_INTERVALS.end();
}

#endif // TIME_UTILS_HPP