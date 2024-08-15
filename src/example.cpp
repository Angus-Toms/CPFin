#include "example.hpp"

// ROADMAP
// 1. Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// 2. Exporting to various formats 
// 3. Plotting

#include <chrono>
#include <vector>

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2000-01-01", "2021-01-01", "1d");
    ps->plot();

    return 0;
}