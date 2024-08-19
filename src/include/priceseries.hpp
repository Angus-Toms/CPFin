#pragma once

#ifndef PRICESERIES_HPP
#define PRICESERIES_HPP

#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <curl/curl.h>
#include <memory>

#include "types.hpp"
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
    
    void plot(const bool includeVolume = false);
    std::vector<std::vector<std::string>> getTableData() const;
    std::string toString(bool includeOverlays = false, bool changeHighlighting = true) const; // TODO: string or pointer?

    // Factory methods ---------------------------------------------------------
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end, const std::string& interval);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::time_t start, const std::string& interval, const std::size_t count);
    static std::unique_ptr<PriceSeries> getPriceSeries(const std::string& ticker, const std::string& start, const std::string& interval, const std::size_t count);

    // Getters -----------------------------------------------------------------
    const std::string getTicker() const;
    const std::vector<std::time_t> getDates() const;
    const std::vector<double> getOpens() const;
    const std::vector<double> getHighs() const;
    const std::vector<double> getLows() const;
    const std::vector<double> getCloses() const;
    const std::vector<double> getAdjCloses() const;
    const std::vector<long> getVolumes() const;

    // Overlays ----------------------------------------------------------------
    void addOverlay(const std::shared_ptr<IOverlay> overlay);
    const std::vector<std::shared_ptr<IOverlay>>& getOverlays() const;

    void addSMA(int period = 20);
    void addEMA(int period = 20, double smoothingFactor = -1);
    void addMACD(int aPeriod = 12, int bPeriod = 26, int cPeriod = 9);
    void addBollingerBands(int period = 20, double numStdDev = 2, MovingAverageType maType = MovingAverageType::SMA);
    void addRSI(int period = 14);

    const std::shared_ptr<SMA> getSMA(int period = 20) const;
    const std::shared_ptr<EMA> getEMA(int period = 20, double smoothingFactor = -1) const;
    const std::shared_ptr<MACD> getMACD(int aPeriod = 12, int bPeriod = 26, int cPeriod = 9) const;
    const std::shared_ptr<BollingerBands> getBollingerBands(int period = 20, double numStdDev = 2, MovingAverageType maType = MovingAverageType::SMA) const;
    const std::shared_ptr<RSI> getRSI(int period = 14) const;

    // temp setters 
    void setDates(std::vector<std::time_t> dates) {
        this->dates = dates;
    }
    void setOpens(std::vector<double> opens) {
        this->opens = opens;
    }
    void setHighs(std::vector<double> highs) {
        this->highs = highs;
    }
    void setCloses(std::vector<double> closes) {
        this->closes = closes;
    }
    void setAdjCloses(std::vector<double> adjCloses) {
        this->adjCloses = adjCloses;
    }
    void setLows(std::vector<double> lows) {
        this->lows = lows;
    }
    void setVolumes(std::vector<long> volumes) {
        this->volumes = volumes;
    }
};
#endif // PRICESERIES_HPP