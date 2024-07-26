#include "example.hpp"
#include "averages.hpp"

// TODO: Pretty printing series (check out fmt library)

int main() {
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-12-31", "1d");
    std::cout << ps.toString() << std::endl;

    auto sma = simpleMovingAverage(ps, 10000);
    for (auto& [date, av] : sma) {
        std::cout << epochToDateString(date) << " : " << av << "\n";
    }

    return 0;
}