#include "example.hpp"

// ROADMAP
// 1. Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// 2. Exporting to various formats 
// 3. Plotting
// 4. Table printing

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-01-31", "1d");
    ps->addSMA(5);
    ps->addEMA(5);
    ps->addBollingerBands(6, 3, MovingAverageType::SMA);
    std::cout << ps->toString(true);

    return 0;
}