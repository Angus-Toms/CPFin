#include "timeseries/ar.hpp"
#include <chrono>

AR::AR(const std::vector<double> data) {
    this->data = data;
}

AR::~AR() {}

void AR::getAutocorrelations() {
    Eigen::Matrix<double, 3, 3> x {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << "x = " << x << std::endl;
}

void AR::solveYuleWalkerEquations(int k) {
}

void AR::train(int k) {
}

std::vector<double> AR::forecast(int steps) {
    return {};
}

std::vector<double> AR::getPhis() const {
    return {};
}

int AR::plot() const {
    return 0;
}

std::string AR::toString() const {
    return "";
}