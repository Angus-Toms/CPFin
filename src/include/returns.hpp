#pragma once 

#ifndef RETURNS_HPP
#define RETURNS_HPP

#include <string>

#include "timeseries.hpp"
#include "priceseries.hpp"
#include "print_utils.hpp"

struct ReturnMetrics {
    double daily;
    double cumulative;
    double annualized;
    double log;

    // Constructor definitions
    // Default constructor
    ReturnMetrics() = default;
    ReturnMetrics(double daily, double cumulative, double annualized, double log)
        : daily(daily), cumulative(cumulative), annualized(annualized), log(log) {}

    std::string toString() const;

    double getDaily() const { return daily; }
    double getCumulative() const { return cumulative; }
    double getAnnualized() const { return annualized; }
    double getLog() const { return log; }
};

class ReturnSeries : public TimeSeries<ReturnMetrics> {
private:
    std::string ticker;

    // Private constructor
    ReturnSeries(const PriceSeries& priceSeries)
        : ticker(priceSeries.getTicker()) {
        getReturns(priceSeries);
    }

    void getReturns(const PriceSeries& priceSeries);

public: 
    // Virtual methods 
    ~ReturnSeries() = default;
    int plot() const override;
    std::string toString() const override;

    //Factory methods 
    static ReturnSeries getReturnSeries(const PriceSeries& priceSeries);
};

#endif // RETURNS_HPP