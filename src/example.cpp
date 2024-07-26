#include "example.hpp"

int main() {
    std::time_t start(1704067200);
    std::time_t end(1706659200);

    // Test all PriceSeries constructors

    // All-argument constructor (date objects)
    PriceSeries ps1 = PriceSeries::getPriceSeries("AAPL", start, end, "1d");
    std::cout << ps1.toString() << std::endl;

    // All-argument constructor (date strings)
    PriceSeries ps2 = PriceSeries::getPriceSeries("AAPL", "2023-12-31", "2024-01-31", "1d");
    std::cout << ps2.toString() << std::endl;

    // No interval constructor (date objects) - defaults to interval of "1d"
    PriceSeries ps3 = PriceSeries::getPriceSeries("AAPL", start, end);
    std::cout << ps3.toString() << std::endl;

    // No interval constructor (date strings) - defaults to interval of "1d"
    PriceSeries ps4 = PriceSeries::getPriceSeries("AAPL", "2023-12-31", "2024-01-31");
    std::cout << ps4.toString() << std::endl;

    // Number of datapoints constructor (date object)
    PriceSeries ps5 = PriceSeries::getPriceSeries("AAPL", start, "1d", 10);
    std::cout << ps5.toString() << std::endl;

    // Number of datapoints constructor (date string)
    PriceSeries ps6 = PriceSeries::getPriceSeries("AAPL", "2023-12-31", "1d", 10);
    std::cout << ps6.toString() << std::endl;

    return 1;
}