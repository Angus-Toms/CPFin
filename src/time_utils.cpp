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
        return -1;
    }
    return time;
}

std::string epochToDateString(const std::time_t time) {
    std::tm* tm = std::localtime(&time);
    if (tm == nullptr) {
        return "Invalid time";
    }
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::time_t intervalToSeconds(const std::string& interval) {
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

bool isInvalidInterval(const std::string& interval) {
    return std::find(VALID_INTERVALS.begin(), VALID_INTERVALS.end(), interval) == VALID_INTERVALS.end();
}