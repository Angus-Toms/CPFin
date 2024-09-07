#pragma once 

#ifndef MA_HPP
#define MA_HPP

#include <nlopt.hpp>

#include <numeric>
#include <vector>
#include <iostream>
#include <cmath>

class MA {
private:
    std::vector<double> data;
    size_t count;
    double mean;

    int k; // Order of MA model
    std::vector<double> theta; // MA coefficients

public:
    MA(const std::vector<double> data);
    ~MA();

    void train(int k);
    // void train(int k); // NOTE: Higher-order MA models
    std::vector<double> forecast(int steps) const;

    std::vector<double> getThetas() const;

    int plot() const;
    std::string toString() const;
};

#endif // MA_HPP