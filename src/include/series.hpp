#pragma once

#ifndef SERIES_HPP
#define SERIES_HPP

#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <curl/curl.h>
#include "json.hpp"
#include "ohclrecord.hpp"
#include "time_utils.hpp"

class PriceSeries {
private:
    std::string ticker;
    std::time_t start;
    std::time_t end;
    std::string interval; // Currently only supports "1d"
    std::map<std::time_t, OHCLRecord> data;

    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    void fetchCSV();
    void parseCSV(const std::string& readBuffer, std::map<std::time_t, OHCLRecord>& data);

public:
    // Full constructor 
    PriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval);
    // No-interval constructor, defaults to "1d"
    PriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end);
    // No-time argument constructor, defaults to 1 year of daily data
    PriceSeries(const std::string& ticker);
    std::string toString();
};

#endif // PRICESERIES_HPP
