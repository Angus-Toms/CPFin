#include "example.hpp"

// ROADMAP
// 1. Sort out exception catching (mainly param checking) for analysis methods
//    Each needs a checkParams function
// 2. Exporting to various formats 
// 3. Plotting

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2001-01-01", "2021-01-31", "1d");
    //std::cout << ps->toString();
    const auto& start = std::chrono::high_resolution_clock::now();
    ps->plot("candlestick", true);
    const auto& end = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "ms" << std::endl;

    return 0;
}