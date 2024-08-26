#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    std::unique_ptr<PriceSeries> ps = std::make_unique<PriceSeries>();
    ps->setCloses({10, 11, 12, 13, 14});
    ps->setDates({0, 1, 2, 3, 4});
    ps->setCount(5);

    auto bollinger = ps->getBollingerBands(5, 1, MovingAverageType::SMA);
    for (const auto& [date, bands] : bollinger->getDataMap()) {
        std::cout << "Date: " << date << ":\n";
        std::cout << "Lower band: " << bands[0] << "\n";
        std::cout << "Middle band: " << bands[1] << "\n";
        std::cout << "Upper band: " << bands[2] << "\n";
    }

    return 0;
}