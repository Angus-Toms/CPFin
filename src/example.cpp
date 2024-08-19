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
    auto ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-02-31", "1d");
    ps->plot(true);
    return 0;
}