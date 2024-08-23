#include "example.hpp"

// ROADMAP
// * Exporting to various formats 

#include <chrono>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"

int main() {
    auto ps = PriceSeries::getPriceSeries("AAPL", "2019-10-01", "2021-01-01", "1d");
    ps->addSMA();
    ps->addEMA();
    ps->addBollingerBands();
    std::cout << ps->toString(true, true);
    ps->exportToCSV();

    return 0;
}