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
  // Test phi values are 1, -2
  std::vector<double> data = {1, 2, -3, 8, -19, -30};
  AR ar(data);

  const auto acs = ar.getAutocorrelations();
  for (size_t i = 0; i < acs.size(); ++i) {
    std::cout << "Autocorrelation at lag " << i << ": " << acs[i] << std::endl;
  }

  // Yule-Walker testing 
  Eigen::MatrixXd acM(2, 2);
  acM(0, 0) = 1.0;
  acM(0, 1) = acs[1];

  acM(1, 0) = acs[1];
  acM(1, 1) = 1.0;

  Eigen::VectorXd acV(2);
  acV(0) = acs[1];
  acV(1) = acs[2];

  Eigen::VectorXd phis = acM.colPivHouseholderQr().solve(acV);
  std::cout << "Phi values: " << phis << std::endl;

  return 0;
}