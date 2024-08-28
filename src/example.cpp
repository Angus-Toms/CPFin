#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-03-31", "1d");
    ps->plot("candlestick", true);

    return 0;
}