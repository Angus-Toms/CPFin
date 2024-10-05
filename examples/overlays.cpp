#include "priceseries.hpp"

int main() {
    const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-06-31", "1d");

    // Simple Moving Average 
    ps->addSMA();   // SMA with default period of 20 
    ps->addSMA(50); // SMA with custom period

    // Exponential Moving Average
    ps->addEMA();   // EMA with default period of 20
    ps->addEMA(50); // EMA with custom period

    // Bollinger Bands
    ps->addBollingerBands();                                // Bollinger Bands with default period of 20, default multiplier of 2, and SMA middle band
    ps->addBollingerBands(15, 1.5, MovingAverageType::EMA); // Bollinger Bands with custom params

    // Moving Average Convergence Divergence
    ps->addMACD();          // MACD with default params
    ps->addMACD(15, 35, 5); // MACD with custom params

    // Relative Strength Index
    ps->addRSI();   // RSI with default period of 14
    ps->addRSI(10); // RSI with custom period

    // Print price series with all overlays 
    std::cout << ps->toString(true);

    return 0;
}