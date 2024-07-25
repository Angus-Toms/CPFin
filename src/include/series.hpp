#pragma once

#ifndef SERIES_HPP
#define SERIES_HPP

#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <curl/curl.h>
#include "json.hpp"
#include "ohclrecord.hpp"
#include "time_utils.hpp"

class PriceSeries {
private:
    std::string ticker;
    std::vector<OHCLRecord> data;
    std::time_t start;
    std::time_t end;
    std::string interval;
    bool includeAdjustedClose;

    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    void fetchCSV();
    void PriceSeries::parseCSVData(const std::string& readBuffer, std::vector<OHCLRecord>& data)

public:
    PriceSeries(const std::string& ticker, std::time_t start, std::time_t end);
    std::string toString();
};

#endif // PRICESERIES_HPP