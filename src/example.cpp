#include "example.hpp"

// ROADMAP
// 1. Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// 2. Exporting to various formats 
// 3. Plotting
// 4. Table printing

#include <chrono>

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-03-31", "1d");

    return 0;
}