#pragma once

#ifndef PRICESERIES_HPP
#define PRICESERIES_HPP

#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <curl/curl.h>
#include <matplot/matplot.h>

#include "time_utils.hpp"
#include "print_utils.hpp"
#include "timeseries.hpp"

// Forward declaration of analysis classes
class SMA;
class EMA;
class ReturnMetrics;

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

    // Getters
    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;
    double getAdjClose() const;
    double getVolume() const;
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
        // Set table printing values
        name = ticker;
        columnHeaders = {"Date", "Open", "High", "Low", "Close", "Adj Close", "Volume"};
        columnWidths = {13, 10, 10, 10, 10, 10, 12};

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
    int plot() const override;
    std::vector<std::vector<std::string>> getAllData() const override;

    // Factory methods ---------------------------------------------------------
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
    static PriceSeries getPriceSeries(const std::string& ticker, const std::string& start, const std::string& interval, const std::size_t count);

    // Getters -----------------------------------------------------------------
    std::string getTicker() const;
    std::map<std::time_t, OHCLRecord> getData() const;

    // Analyses ----------------------------------------------------------------
    // Simple moving average 
    SMA getSMA(int window = 20) const;

    // Exponential moving average 
    EMA getEMA(int window = 20, double smoothingFactor = -1) const;

    // Returns 
    ReturnMetrics getReturns() const;    
    
};

// class MultiPriceSeries : public TimeSeries<std::vector<OHCLRecord>> {
// private:
//     std::vector<std::string> tickers;
//     std::time_t start;
//     std::time_t end;
//     std::string interval;

//     // Private constructor 
//     MultiPriceSeries(const std::vector<std::string>& tickers, const std::time_t start, const std::time_t end, std::string& interval)
//         : tickers(tickers), start(start), end(end), interval(interval) {
//         // Set table printing values 
//         name = "";
//         columnHeaders = {"Date"};
//         columnWidths = {13};
//         std::vector<std::string> headers = {"Open", "High", "Low", "Close", "Adj Close", "Volume"};
//         std::vector<int> widths = {10, 10, 10, 10, 10, 12};
//         for (const auto& ticker : tickers) {
//             name += ticker;
//             columnHeaders.insert(columnHeaders.end(), headers.begin(), headers.end());
//             columnWidths.insert(columnWidths.end(), widths.begin(), widths.end());
//         }
//         checkArguments();
//         fetchCSV();
//     }

//     static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
//         ((std::string*)userp)->append((char*)contents, size * nmemb);
//         return size * nmemb;
//     }
//     void checkArguments();
//     void fetchCSV();
//     void parseCSV(const std::string& readBuffer, std::map<std::time_t, std::vector<OHCLRecord>>& data);

// public:
//     // Virtual methods 
//     ~MultiPriceSeries() = default;
//     int plot() const override;
//     std::vector<std::vector<std::string>> getAllData() const override;

//     // Factory methods ---------------------------------------------------------
//     // All-argument constructor (date objects)
//     static MultiPriceSeries getMultiPriceSeries(const std::vector<std::string>& tickers, const std::time_t start, const std::time_t end, const std::string& interval); 
//     // All-argument constructor (date strings)
//     static MultiPriceSeries getMultiPriceSeries(const std::vector<std::string>& tickers, const std::string& start, const std::string& end, const std::string& interval);
//     // No interval constructor (date objects) - defaults to interval of "1d"
//     static MultiPriceSeries getMultiPriceSeries(const std::vector<std::string>& tickers, const std::time_t start, const std::time_t end);
//     // No interval constructor (date strings) - defaults to interval of "1d"
//     static MultiPriceSeries getMultiPriceSeries(const std::vector<std::string>& tickers, const std::string& start, const std::string& end);
//     // Number of datapoints constructor (date object)
//     static MultiPriceSeries getMultiPriceSeries(const std::vector<std::string>& tickers, const std::time_t start, const std::string& interval, const std::size_t count);
//     // Number of datapoints constructor (date string)
//     static MultiPriceSeries getMultiPriceSeries(const std::vector<std::string>& tickers, const std::string& start, const std::string& interval, const std::size_t count);

//     // Getters -----------------------------------------------------------------
//     std::string getTicker() const;
//     std::map<std::time_t, std::vector<OHCLRecord>> getData() const;
// };

#endif // PRICESERIES_HPP