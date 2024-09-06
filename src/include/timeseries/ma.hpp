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

    double theta; // MA(1) coefficient

    // Helper functions for optimisation
    // double objFunction(const std::vector<double>& x, std::vector<double>& grad, void *data) const;
    // double getNLL(const std::vector<double> params, const std::vector<double> data) const;

public:
    MA(const std::vector<double> data);
    ~MA();

    void train();
    // void train(int k); // NOTE: Higher-order MA models
    std::vector<double> forecast(int steps) const;

    std::vector<double> getThetas() const;

    int plot() const;
    std::string toString() const;
};

#endif // MA_HPP