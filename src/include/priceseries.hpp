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
class ReturnMetrics;
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

    // Analyses ----------------------------------------------------------------
    // Simple moving average 
    std::unique_ptr<SMA> getSMA(int window = 20) const;
    // Exponential moving average 
    std::unique_ptr<EMA> getEMA(int window = 20, double smoothingFactor = -1) const;
    // Returns 
    std::unique_ptr<ReturnMetrics> getReturns() const;  
    // Moving-Average Convergence/Divergence 
    std::unique_ptr<MACD> getMACD(int aPeriod = 12, int bPeriod = 26, int cPeriod = 9) const;
    // Bollinger bands
    std::unique_ptr<BollingerBands> getBollingerBands(int window = 20, 
                                                            double numStdDev = 2, 
                                                            MovingAverageType maType = MovingAverageType::SMA) const;
    // Relative Strength Index 
    std::unique_ptr<RSI> getRSI(int period = 14);
    // Standard deviations 
    double getStdDev() const;  
};

#endif // PRICESERIES_HPP