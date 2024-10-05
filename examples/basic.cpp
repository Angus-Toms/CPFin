#include "priceseries.hpp"

int main() {
    // Get historical price data
    const auto ps = PriceSeries::getPriceSeries(
        "AAPL",         // Ticker symbol
        "2020-01-01",   // Start date
        "2020-03-31",   // End date
        "1d"            // Interval
    );  

    // Add overlays 
    ps->addSMA();
    
    // Print data
    std::cout << ps->toString();

    // Plot data
    ps->plot();

    return 0;    
}