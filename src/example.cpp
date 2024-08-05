#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"

// TODO: Extract printing routines? Sort column widths etc. 
// TODO: Sort out exception catching (mainly param checking)


int main() {
    // Get data
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-01-31", "1d");
    std::cout << ps.toString() << std::endl;

    EMA ema = EMA(ps, 20);
    std::cout << ema.toString() << std::endl;

    SMA sma = SMA(ps, 50);
    std::cout << sma.toString() << std::endl;

    ReturnMetrics returns = ps.getReturns();
    std::cout << returns.toString() << std::endl;

    return 0;
}