#include "example.hpp"

// ROADMAP
// * Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// * Exporting to various formats 

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2019-10-01", "2021-01-01", "1d");
    ps->addSMA(7000);
    ps->addEMA(-10);
    ps->addBollingerBands(20, -10);
    ps->addMACD(-1, -2, -3);

    return 0;
}