#include "example.hpp"

int main() {

	const auto ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-12-31");
	ps->plot();
	std::cout << ps->toString();

	// Test Time Series analyses
	const auto ar = ps->getAR(5);
	std::cout << ar->toString();
	ar->plot();

	return 0;
}