#include "priceseries.hpp"

int main() {
    // Get price series with overlays
    const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-03-31", "1d");
    ps->addSMA();
    ps->addRSI();

    ps->plot(
        "candlestick",      // Plot type: "candlestick", "line", or "area"
        false               // Include volume subplot?
    );

    // Specify a filename to save plots 
    ps->plot("candlestick", true, "imgs/candlestick.png");
    ps->plot("line", true, "imgs/line.png");
    ps->plot("area", true, "imgs/area.png");

    return 0;
}