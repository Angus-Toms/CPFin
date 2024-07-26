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
        double open;
        double high;
        double close;
        double low;
        double adjClose;
        double volume;

    public:
        // Constructors
        OHCLRecord();
        OHCLRecord(double open, double high, double close, double low, double adjClose, double volume);
        std::string toString() const;

        // Getters
        double getClose() const;
};

#endif // OHCLRECORD_HPP