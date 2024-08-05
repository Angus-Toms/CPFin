#pragma once 

#ifndef RETURNS_HPP
#define RETURNS_HPP

#include <string>

#include "timeseries.hpp"
#include "print_utils.hpp"

class PriceSeries;

struct ReturnMetricRecord {
    double daily;
    double cumulative;
    double annualized;
    double log;

    ReturnMetricRecord() = default;
    ReturnMetricRecord(double daily, double cumulative, double annualized, double log)
        : daily(daily), cumulative(cumulative), annualized(annualized), log(log) {}

    double getDaily() const { return daily; }
    double getCumulative() const { return cumulative; }
    double getAnnualized() const { return annualized; }
    double getLog() const { return log; }
};

class ReturnMetrics : public TimeSeries<ReturnMetricRecord> {
private:
    const PriceSeries& priceSeries;

    void calculate();

public:
    // Constructor
    ReturnMetrics(const PriceSeries& priceSeries);

    // Getters -----------------------------------------------------------------
    std::vector<double> getDailys() const;
    std::vector<double> getCummulatives() const;
    std::vector<double> getAnnualizeds() const;
    std::vector<double> getLogs() const;

    // Implement virtual methods from TimeSeries
    int plot() const override;
    std::vector<std::vector<std::string>> getTableData() const override;
};

#endif // RETURNS_HPP