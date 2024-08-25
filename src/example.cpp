#include "example.hpp"

// ROADMAP
// * Exporting to various formats 

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
    ps->exportToCSV("AAPL_2020.csv");
    std::cout << ps->toString();

    return 0;
}