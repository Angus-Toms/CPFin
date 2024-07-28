#pragma once

#ifndef SERIES_HPP
#define SERIES_HPP

#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <curl/curl.h>
#include "time_utils.hpp"
#include "timeseries.hpp"

struct OHCLRecord {
    double open;
    double high;
    double low;
    double close;
    double adjClose;
    double volume;

    // Constructor definitions
    // Default constructor
    OHCLRecord() = default;
    OHCLRecord(double open, double high, double low, double close, double adjClose, double volume);
    std::string toString() const;
    double getClose() const;
};

class PriceSeries : public TimeSeries<OHCLRecord> {
private:
    std::string ticker;
    std::time_t start;
    std::time_t end;
    std::string interval;

    // Private constructor 
    PriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval)
        : ticker(ticker), start(start), end(end), interval(interval) {
        checkArguments();
        fetchCSV();
    }

    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    void checkArguments();
    void fetchCSV();
    void parseCSV(const std::string& readBuffer, std::map<std::time_t, OHCLRecord>& data);

public:
    // Virtual methods 
    ~PriceSeries() = default;
    void plot() const override;
    std::string toString() const override;

    // Factory methods 
    // All-argument constructor (date objects)
    static PriceSeries getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval);
    // All-argument constructor (date strings)
    static PriceSeries getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end, const std::string& interval);
    // No interval constructor (date objects) - defaults to interval of "1d"
    static PriceSeries getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end);
    // No interval constructor (date strings) - defaults to interval of "1d"
    static PriceSeries getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end);
    // Number of datapoints constructor (date object)
    static PriceSeries getPriceSeries(const std::string& ticker, const std::time_t start, const std::string& interval, const std::size_t count);
    // Number of datapoints constructor (date string)
    static PriceSeries getPriceSeries(const std::string& ticker, const std::string &start, const std::string &interval, const std::size_t count);

    // Getters
    std::map<std::time_t, OHCLRecord> getData() const;
    OHCLRecord getRecord(const std::time_t date) const;
    
};

#endif // PRICESERIES_HPP
