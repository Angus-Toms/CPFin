#pragma once

#ifndef OHCLRECORD_HPP
#define OHCLRECORD_HPP

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

class OHCLRecord {
    private:
        std::time_t date;
        double open;
        double high;
        double close;
        double low;

    public:
        OHCLRecord(std::time_t date, double open, double high, double close, double low);
        std::string toString() const;
};

#endif // OHCLRECORD_HPP