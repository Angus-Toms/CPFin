#include "priceseries.hpp"

int main() {
    // Training an AR model on Google prices -----------------------------------
    auto ps = PriceSeries::getPriceSeries("GOOG", "2023-01-01", "2023-02-31", "1d");

    // Train AR(5) and check out its learnt parameters
    auto ar = ps->getAR(5);
    std::cout << ar->toString();

    // Forecast prices for the next 10 days
    ar->forecast(10);
    ar->plot();

    // Training an MA model on Apple prices ------------------------------------ 
    auto ps2 = PriceSeries::getPriceSeries("AAPL", "2015-01-01", "2015-02-31", "1d");

    // Train an MA model and inspect its learnt parameters
    auto ma = ps2->getMA(5);
    std::cout << ma ->toString();

    // Forecast prices for the next 10 days
    ma->forecast(10);
    ma->plot();

    // Training an ARMA model on Microsoft prices ------------------------------
    auto ps3 = PriceSeries::getPriceSeries("MSFT", "2010-01-01", "2010-12-31", "1d");

    // Train an MA model and inspect its learnt parameters
    auto arma = ps3->getARMA(3, 3);
    std::cout << ma ->toString();

    // Forecast prices for the next 60 days
    ma->forecast(60);
    ma->plot();

    return 0;
}