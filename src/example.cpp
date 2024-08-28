#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
    std::cout << ps->toString();

    return 0;
}