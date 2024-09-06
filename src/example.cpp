#include "example.hpp"

#include <chrono>
#include <iostream>
#include "overlays/sma.hpp"
#include "overlays/bollinger.hpp"
#include "timeseries/ar.hpp"
#include "timeseries/ma.hpp"

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
  // Test mu, theta values are -1, 1.5
  std::vector<double> data = {1.2967141530112327, 0.5623928682265689, 1.4753413983349295, 2.193492148787887, 0.26124065399505897, 0.6126937179954867, 2.4260402068972273, 1.2515921660514304, 0.17703866823446618, 1.4364549207729553, 0.22807029847034485, 0.42695378499223563, 1.1351082222800855, -1.1616726989710047, -0.5422617835814731, 0.5826960372616339, -0.10037361448622921, 1.3168135566621588, -0.1708735420402656, -0.43069888623104924, 2.5481095091886123, 0.2810939457291536, 0.9126834647852311, -0.6382538271510415, 0.5405669127175088, 1.0197991346149027, -0.373178095364276, 1.4058967338301325, 0.12422170641206066, 0.6284339881904842, 0.25663213772925847, 2.7726195069548174, 0.41604713836027857, -0.2550114840083135, 1.834087097894369, -0.58535263239166, 1.25303232499896, -1.2014428428807267, -0.13625202412247533, 1.2624984456488098, 1.4990943328215858, 0.8236749651908885, 0.6500780613737654, 0.5220259608883594, -0.6183012512495696, 0.3758601896787769, 0.48333007071915424, 1.9492499804108734, 0.9321938443246783, -1.0317638132764262, 1.4766920004673418, 0.3501009257047245, 0.20009445577730461, 1.5470606889020597, 1.7086642647277774, 1.5250802146170084, -0.22547354704587821, 0.6586311287933132, 1.193105906573807, 1.7092924408416463, 0.12571673673023825, 0.7101758709052409, -0.2692031786732647, -0.1749396292794651, 1.8517671472103323, 1.9937348640919834, 0.45674187270550165, 1.817934922208091, 0.9609294454692294, 0.08255304038534894, 1.290419556429439, 2.265757445364286, 0.45656664759685467, 2.3718088636359966, -2.132673835252546, 2.145851525193173, 0.7226665673631265, 0.4835832358864983, 0.9515622466286758, -1.2059210699079932, 0.9778418950826667, 1.2010469490792488, 2.206471530439167, -0.013849027221950583, 0.09516044076154191, 0.45994167699410105, 1.8157535264189815, 0.9456706861192696, 0.20448957430102432, 1.4192194738667638, 0.7944240627253691, 1.749229480663281, -0.09578209198393017, 0.6127484721777023, 0.4734242761873961, -0.5850933175056869, 1.3888232666909999, 1.001831216766974, 0.7529024022064831, 0.5643901752963609};
  MA ma(data);

  ma.train();

  return 0;
}