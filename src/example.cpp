#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"
#include "timeseries/ar.hpp"

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

  // Test Eigen 
  std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  AR ar(data);
  ar.getAutocorrelations();

  return 0;
}