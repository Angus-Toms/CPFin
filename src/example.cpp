#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-12-31", "1d");
    ps->addBollingerBands(20, 1, MovingAverageType::SMA);
    ps->addBollingerBands(20, 3, MovingAverageType::SMA);
    ps->addBollingerBands(20, 5, MovingAverageType::SMA);
    ps->plot("candlestick", true);

    return 0;
}