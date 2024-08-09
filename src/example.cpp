#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"
#include "bollinger.hpp"

// TODO: Extract printing routines? Sort column widths etc. 
// TODO: Sort out exception catching (mainly param checking)


int main() {
    // Get data
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-03-31", "1d");
    // std::cout << ps.toString() << "\n";

    MACD macd = ps.getMACD();
    std::cout << macd.toString() << "\n";

    return 0;
}