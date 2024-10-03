#include "example.hpp"

int main() {
	std::vector<double> arma_data = {
		1, 2, 3
	};

	std::vector<std::time_t> dates;
	for (int i = 0; i < arma_data.size(); ++i) {
		dates.push_back(dateStringToEpoch("2022-01-01") + (intervalToSeconds("1d") * i));
	}

	auto ps = PriceSeries();
	ps.setCloses(arma_data);
	ps.setDates(dates);

	auto arma = ps.getARMA(2, 2);
	std::cout << arma->toString();

	return 0;
}