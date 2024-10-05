<div style="text-align: center;">
  <a href="https://github.com/Angus-Toms/cpp_finance/actions/workflows/ci.yml">
    <img src="https://github.com/Angus-Toms/cpp_finance/actions/workflows/ci.yml/badge.svg" alt="CI">
  </a>
</div>

# CPFin
**CPFin** is a C++ library for financial analysis and currently supports historical data downloads, creation of custom indicators, and plotting and exporting to various formats.

## QuickStart Guide 
To download data, construct some indicators, and export the results; run the following:
```cpp
#include "priceseries.hpp"

int main() {
  // Fetch data
  const auto ps = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-12-31");

  // Add indicators
  ps->addRSI();
  ps->addBollingerBands();

  // View and export results 
  ps->plot();
  ps->exportCSV("aapl.csv");

  return 0;
}
```
For more examples, check the [`examples`](https://github.com/Angus-Toms/CPFin/tree/main/examples) directory and refer to the [`README.md`](https://github.com/Angus-Toms/CPFin/tree/main/examples#readme) for instructions on running them.

## Download
Clone the project with the following command:
```sh
git clone https://github.com/Angus-Toms/CPFin
```
### Prerequisites
We are trying to minimise the number of dependencies required. Most are shipped with CPFin in the `third_party` dir. The current distribution does require working distributions of the fmt and NLOpt libraries as well as Python3.

## Testing 
To run tests, navigate to the root directory, and call:
```sh
make test
```

## Roadmap
- [X] Price scraper 
- [X] Basic indicators 
- [X] Pretty printing and plotting 
- [X] Exporting to various formats (.csv, .png) 
- [ ] More technical analysis
- [ ] Reduce need for prerequisites
- [ ] More stringent tests
- [ ] Better documentation, website

## Contributing 
Contributions, questions, and issues found are all welcome. Please submit a PR or open an issue. Thank you! 