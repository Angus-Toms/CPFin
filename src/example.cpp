#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"
#include "bollinger.hpp"
#include "rsi.hpp"

// ROADMAP
// 1. Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// 2. Exporting to various formats 
// 3. Plotting

#include <chrono>
#include <vector>

int main() {
    std::unique_ptr<PriceSeries> ps = PriceSeries::getPriceSeries("AAPL", "2000-01-01", "2021-01-01", "1d");

    return 0;
}