#pragma once 

#ifndef ARMA_HPP
#define ARMA_HPP

#include <nlopt.hpp>

#include <numeric>
#include <vector>
#include <iostream>
#include <cmath>

struct ARMAData {
    std::vector<double> data;
    int p;
    int q;
};

class ARMA {
private:
    std::vector<double> data;
    size_t count;
    double mean;

    int arOrder;
    int maOrder;
    std::vector<double> phis; // AR coefficients
    std::vector<double> thetas; // MA coefficients

public:
    ARMA(const std::vector<double> data);
    ~ARMA();

    void train(int arOrder, int maOrder);
    // void train(); NOTE: auto-determine AR and MA orders
    std::vector<double> forecast(int steps) const;

    double getMean() const;
    std::vector<double> getPhis() const;
    std::vector<double> getThetas() const;

    int plot() const;
    std::string toString() const;
};

#endif // ARMA_HPP