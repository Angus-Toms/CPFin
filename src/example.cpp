#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"
#include "timeseries/ar.hpp"
#include "timeseries/ma.hpp"
#include "timeseries/arma.hpp"

#include "priceseries.hpp"

int main() {
  // // Fetch data
  // const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-12-31");

  // // Add indicators
  // ps->addRSI();
  // ps->addBollingerBands();

  // // View and export results 
  // ps->plot();
  // ps->exportCSV("aapl.csv");

  // Example AR model, X_t = 2X_{t-1} - 1X_{t-2}
  std::vector<double> data = {0.0, 2.429982153980172, 0.24898100823934854, -1.2209794074070102};
  MA ma(data);
  ma.train(2);
  ma.forecast(3);

  return 0;
}