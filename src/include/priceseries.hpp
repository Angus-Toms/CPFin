#pragma once

#ifndef PRICESERIES_HPP
#define PRICESERIES_HPP

#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <curl/curl.h>
#include <memory>

#include "enums.hpp"
#include "time_utils.hpp"
#include "print_utils.hpp"
#include "timeseries.hpp"

#include "matplotlibcpp.h"

// Forward declaration of indicator classes
class SMA;
class EMA;
class MACD;
class BollingerBands;
class RSI;

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
    
    std::vector<std::unique_ptr<TimeSeries>> overlays;

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
    std::vector<std::vector<std::string>> getTableData() const override;

     // Factory methods --------------------------------------------------------
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end, const std::string& interval);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::time_t start, const std::string& interval, const std::size_t count);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::string& start, const std::string& interval, const std::size_t count);

    // Getters -----------------------------------------------------------------
    std::string getTicker() const;
    std::vector<std::time_t> getDates() const;
    std::vector<double> getOpens() const;
    std::vector<double> getHighs() const;
    std::vector<double> getLows() const;
    std::vector<double> getCloses() const;
    std::vector<double> getAdjCloses() const;
    std::vector<double> getVolumes() const;

    // Overlays ----------------------------------------------------------------
    template <typename T>
    void addOverlay(std::unique_ptr<TimeSeries<T>> overlay) {
        overlays.push_back(std::move(overlay));
    }
    // Simple moving average 
    void getSMA(int period = 20);
    // Exponential moving average 
    void getEMA(int period = 20, double smoothingFactor = -1);
    // Returns 
    void getReturns();  
    // Moving-Average Convergence/Divergence 
    void getMACD(int aPeriod = 12, int bPeriod = 26, int cPeriod = 9);
    // Bollinger bands
    void getBollingerBands(int period = 20, double numStdDev = 2, 
                           MovingAverageType maType = MovingAverageType::SMA);
    // Relative Strength Index 
    void getRSI(int period = 14);
};

#endif // PRICESERIES_HPP