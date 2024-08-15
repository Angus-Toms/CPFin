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

#include "matplotlibcpp.h"

// Forward declaration of overlays 
class IOverlay;
class SMA;
class EMA;
class MACD;
class BollingerBands;
class RSI;

class PriceSeries {
private:
    std::string ticker;
    std::time_t start;
    std::time_t end;
    std::string interval;

    std::vector<std::time_t> dates;
    std::vector<double> opens;
    std::vector<double> highs;
    std::vector<double> lows;
    std::vector<double> closes;
    std::vector<double> adjCloses;
    std::vector<long> volumes;

    std::vector<std::shared_ptr<IOverlay>> overlays;

    // Private constructor
    PriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval);

    static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    void checkArguments();
    void fetchCSV();
    void parseCSV(const std::string& readBuffer);

public:
    PriceSeries();
    ~PriceSeries();

    // // Move constructor
    // PriceSeries(PriceSeries&& other) noexcept = default;

    // // Move assignment operator
    // PriceSeries& operator=(PriceSeries&& other) noexcept = default;
    
    void plot() const;
    std::string toString() const;

    // Factory methods ---------------------------------------------------------
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
    std::vector<long> getVolumes() const;

    // Overlays ----------------------------------------------------------------
    void addOverlay(std::shared_ptr<IOverlay> overlay);
    const std::vector<std::shared_ptr<IOverlay>>& getOverlays() const;

    void addSMA(int period = 20);
    void addEMA(int period = 20, double smoothingFactor = -1);
    void addMACD(int aPeriod = 12, int bPeriod = 26, int cPeriod = 9);
    void addBollingerBands(int period = 20, double numStdDev = 2, const std::string& maType = "sma");
    void addRSI(int period = 14);
};
#endif // PRICESERIES_HPP