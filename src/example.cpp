#include "example.hpp"
#include "averages.hpp"

// TODO: Pretty printing series (check out fmt library)
// TODO: Sort out exception catching (mainly param checking)

int main() {
    // Test out SMA
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-04-31", "1d");
    std::cout << ps.toString() << std::endl;
    
    // SimpleMovingAverage sma = SimpleMovingAverage::getSimpleMovingAverage(ps, 2);
    // std::cout << sma.toString() << std::endl;

    return 0;
}