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

  // Eigen test 
  std::vector<double> data = {-15, 25, -13, 54, 75};
  AR ar(data);

  Eigen::VectorXd phis = ar.solveYuleWalkerEquations(4);
  for (int i = 0; i < 4; i++) {
    std::cout << "phi(" << i << "): " << phis(i) << std::endl;
  }

  return 0;
}