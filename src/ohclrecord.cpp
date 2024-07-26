#include "ohclrecord.hpp"
#include "time_utils.hpp"

OHCLRecord::OHCLRecord() {
    open = 0.0;
    high = 0.0;
    low = 0.0;
    close = 0.0;
    adjClose = 0.0;
    volume = 0.0;
}

OHCLRecord::OHCLRecord(double open, double high, double low, double close, double adjClose, double volume) {
    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;
    this->adjClose = adjClose;
    this->volume = volume;
}

std::string OHCLRecord::toString() const {
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(2) << open << " | "
        << std::fixed << std::setprecision(2) << high << " | "
        << std::fixed << std::setprecision(2) << low << " | "
        << std::fixed << std::setprecision(2) << close << " | "
        << std::fixed << std::setprecision(2) << adjClose << " | "
        << std::fixed << std::setprecision(2) << volume << " |";

    return oss.str();
};