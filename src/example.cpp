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

    auto ps2 = PriceSeries::getPriceSeries("GOOG", "2019-10-01", "2021-01-01", "1d");
    ps2->addSMA(5);
    ps2->addEMA(5);
    ps2->addRSI();
    ps2->addMACD();
    ps2->plot("line", true);


    return 0;
}