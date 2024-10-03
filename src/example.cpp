#include "example.hpp"
#include <chrono>

int main() {
	const auto ps = PriceSeries::getPriceSeries("AAPL", "2010-01-01", "2011-01-01");
	const auto ps2 = PriceSeries::getPriceSeries("AAPL", "2011-01-01", "2011-01-31");

	const auto start = std::chrono::high_resolution_clock::now();
	auto arma = ps->getARMA(10, 10);
	std::cout << arma->toString() << std::endl;
	arma->forecast(30);
	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Time taken: " << duration << "micros" << std::endl;
	arma->plot();
	ps2->plot();

	return 0;
}