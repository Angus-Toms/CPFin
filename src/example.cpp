#include "example.hpp"

// ROADMAP
// 1. Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// 2. Exporting to various formats 
// 3. Add area plot
// 4. Sort MACD and RSI subplots

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2021-01-01", "1d");
    ps->plot("candlestick", true);

    return 0;
}