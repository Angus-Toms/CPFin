#include "ohclrecord.hpp"
#include "time_utils.hpp"

OHCLRecord::OHCLRecord(std::time_t date, double open, double high, double close, double low) {
    this->date = date;
    this->open = open;
    this->high = high;
    this->close = close;
    this->low = low;
}

std::string OHCLRecord::toString() const {
    std::ostringstream oss;

    oss << "| " << epochToDateString(date) << " | " 
        << std::fixed << std::setprecision(2) << open << " | "
        << std::fixed << std::setprecision(2) << high << " | "
        << std::fixed << std::setprecision(2) << close << " | "
        << std::fixed << std::setprecision(2) << low << " |";

    return oss.str();
};