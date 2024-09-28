#include "example.hpp"

int main() {
	const auto ps = PriceSeries::getPriceSeries("AAPL", "2021-01-01", "2021-12-31");
	std::cout << ps->toString();

	return 0;
}