#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"
#include "bollinger.hpp"

// TODO: Extract printing routines? Sort column widths etc. 
// TODO: Sort out exception catching (mainly param checking)


int main() {
    // Get data
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-01-21", "1d");
    std::cout << ps.toString() << std::endl;

    SMA sma = ps.getSMA(3);
    std::cout << sma.toString() << std::endl;

    BollingerBands bb = ps.getBollingerBands(3, 2, MovingAverageType::SMA);
    std::cout << bb.toString() << std::endl; 

    return 0;
}