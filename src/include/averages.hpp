#pragma once 

#ifndef AVERAGES_HPP 
#define AVERAGES_HPP

#include <memory>

#include "timeseries.hpp"

class PriceSeries;

struct MACDRecord {
    double macdLine; // a-day EMA - b-day EMA
    double signalLine; // c-day EMA of MACD line 
    double histogram; // MACD - signal

    // Constructors 
    MACDRecord() = default;
    MACDRecord(double macdLine, double signalLine, double histogram)
        : macdLine(macdLine), signalLine(signalLine), histogram(histogram) {}
};

class SMA : public TimeSeries<double> {
private:
    const PriceSeries& priceSeries;
    int window;

    void calculate();

public:
    SMA(const PriceSeries& priceSeries, int window = 20);

    // Implement virtual methods from TimeSeries
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

class EMA : public TimeSeries<double> {
private:
    const PriceSeries& priceSeries;
    int window;
    double smoothingFactor;

    void calculate();

public:
    EMA(const PriceSeries& priceSeries, int window = 20, double smoothingFactor = -1);

    // Implement virtual methods from TimeSeries 
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

class MACD : public TimeSeries<MACDRecord> {
private:
    const PriceSeries& priceSeries;
    int aPeriod, bPeriod, cPeriod;

    void calculate();
public:
    MACD(const PriceSeries& priceSeries, int aPeriod = 12, int bPeriod = 26, int cPeriod = 9);

    // Implement virtual methods from TimeSeries 
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

#endif // AVERAGES_HPP