#pragma once 

#ifndef AR_HPP
#define AR_HPP 

#include <vector>
#include <iostream>
#include <numeric>

#include "../../third_party/Eigen/Dense"

class AR {
private:
    std::vector<double> data;
    int k; // AR model order
    std::vector<double> phis; // AR coefficients
    double mean;
    double variance;
    size_t count;

    std::vector<double> autocorrelations;
    std::vector<double> partial_autocorrelations;

public:
    AR(const std::vector<double> data);
    ~AR();
    
    void train(int k);
    // void train(); NOTE: auto-determine k based on significance of PACF values 
    std::vector<double> forecast(int steps) const;
    // void forecast(std::time_t end); NOTE: forecast until a certain future date

    std::vector<double> getPhis() const;
    std::vector<double> getAutocorrelations() const;

    int plot() const;
    std::string toString() const;
};

#endif // AR_HPP