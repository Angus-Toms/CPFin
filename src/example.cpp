#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"

// TODO: Extract printing routines? Sort column widths etc. 
// TODO: Sort out exception catching (mainly param checking)


int main() {
    // Get data
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-04-31", "1d");
    std::cout << ps.toString() << std::endl;

    // Test analysis and printing
    SMA sma1 = ps.getSMA();
    SMA sma2 = ps.getSMA(10);
    std::cout << sma1.toString() << std::endl;
    std::cout << sma2.toString() << std::endl;

    ReturnMetrics returns = ps.getReturns();
    std::cout << returns.toString() << std::endl;

    return 0;
}