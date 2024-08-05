#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"

// TODO: Extract printing routines? Sort column widths etc. 
// TODO: Sort out exception catching (mainly param checking)


int main() {
    // Get data
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-01-31", "1d");
    std::cout << ps.toString() << std::endl;

    // Test EMA constructions 
    // No args specified, 20d and standard alpha 
    EMA ema1 = ps.getEMA(10);
    std::cout << ema1.toString() << std::endl;

    SMA sma1 = ps.getSMA(10);
    std::cout << sma1.toString() << std::endl;


    // ReturnMetrics returns = ps.getReturns();
    // std::cout << returns.toString() << std::endl;

    return 0;
}