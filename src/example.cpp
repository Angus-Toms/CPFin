#include "example.hpp"

int main() {
    std::time_t start(1704067200);
    std::time_t end(1706659200);

    // Full constructor 
    PriceSeries fullSeries("AAPL", start, end, "1wk");
    std::cout << fullSeries.toString() << "\n";

    // No-interval constructor (should default to "1d")
    PriceSeries seriesNoInterval("AAPL", start, end);
    std::cout << seriesNoInterval.toString() << "\n";

    // No-time argument constructor (should default to 1 year of daily data)
    PriceSeries seriesNoTime("AAPL");
    std::cout << seriesNoTime.toString() << "\n";

    return 1;
}