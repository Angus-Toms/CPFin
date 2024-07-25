#pragma once 

#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

std::time_t dateStringToEpoch(const std::string& dateStr);
std::string epochToDateString(std::time_t time);

#endif // TIME_UTILS_HPP