#include "time_utils.hpp"

std::string epochToDateString(const std::time_t date, bool includeTime) {
    std::tm* tm = std::localtime(&date);
    if (tm == nullptr) {
        return "Invalid time";
    }
    std::ostringstream oss;
    if (includeTime) {
        oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    } else {
        oss << std::put_time(tm, "%Y-%m-%d");
    }
    return oss.str();
}

std::time_t dateStringToEpoch(const std::string& dateStr) {
    // TODO: Use fmt
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
        return -1;
    }
    return time;
}