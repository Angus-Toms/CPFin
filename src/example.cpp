#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"
#include "bollinger.hpp"
#include "rsi.hpp"

// TODO: Should getIndicator methods be returning pointers?
// TODO: Sort out exception catching (mainly param checking)

#include <chrono>
#include <vector>

int main() {
    std::unique_ptr<PriceSeries> ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2021-01-01", "1d");
    

    return 0;
}