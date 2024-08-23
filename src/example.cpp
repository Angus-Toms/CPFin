#include "example.hpp"

// ROADMAP
// * Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// * Exporting to various formats 
// * Add area plot
// * Sort (subplot) spacing 
// * Volume plot yaxis labelling

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2019-10-01", "2021-01-01", "1d");
    ps->addSMA(5);
    ps->addEMA(5);
    ps->addRSI();
    ps->addMACD();
    ps->plot("area", true);
    ps->plot("line", true);
    ps->plot("candlestick", true);

    return 0;
}