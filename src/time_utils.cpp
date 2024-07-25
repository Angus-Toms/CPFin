#include "time_utils.hpp"

std::time_t dateStringToEpoch(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);

    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        std::cerr << "Failed to parse date string " << dateStr << std::endl;
        return -1;
    }

    std::time_t time = std::mktime(&tm);
    if (time == -1) {
        std::cerr << "Error creating time_t object" << std::endl;
    }
    return time;
}

std::string epochToDateString(std::time_t time) {
    std::tm* tm = std::localtime(&time);
    if (tm == nullptr) {
        return "Invalid time";
    }
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}