#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"

// TODO: Extract printing routines? Sort column widths etc. 
// TODO: Sort out exception catching (mainly param checking)


int main() {
    // Get data
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-04-31", "1d");
    std::cout << ps.toString() << std::endl;

    return 0;
}