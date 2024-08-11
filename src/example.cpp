#include "example.hpp"
#include "averages.hpp"
#include "returns.hpp"
#include "bollinger.hpp"
#include "rsi.hpp"

// TODO: Should getIndicator methods be returning pointers?
// TODO: Sort out exception catching (mainly param checking)


int main() {
    PriceSeries ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-01-15", "1d");
    std::cout << ps.toString() << "\n";

    ReturnMetrics returns = ps.getReturns();
    std::cout << returns.toString() << "\n";

    RSI rsi = ps.getRSI(3);
    std::cout << rsi.toString() << "\n";

    return 0;
}