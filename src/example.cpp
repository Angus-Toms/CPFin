#include "example.hpp"

int main() {
	// const auto ps = PriceSeries::getPriceSeries("MSFT", "2022-01-01", "2022-12-31", "1d");

	// auto arma = ps->getARMA(3, 3);
	// std::cout << arma->toString();
	// arma->forecast(60);
	// arma->plot();

	const auto ps = PriceSeries::getPriceSeries("MSFT", "2010-01-01", "2010-12-31", "1d");

	auto arma = ps->getARMA(3, 3);
	std::cout << arma->toString();
	arma->forecast(60);
	arma->plot();

	return 0;
}