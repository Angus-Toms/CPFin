#include "example.hpp"

int main() {
    std::time_t start(1704067200);
    std::time_t end(1706659200);
    PriceSeries series = PriceSeries("AAPL", start, end);
    std::cout << series.toString() << "\n";

    return 1;
}