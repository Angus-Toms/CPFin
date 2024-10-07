std::tuple<double, double> priceSeriesFromTxt(const std::string& filename, int model, int order) {
	std::cout << "Reading from " << filename << "\n";
	// model: 0 = AR, 1 = MA, 2 = ARMA

	std::vector<double> closes;
	std::vector<double> testSet;
	std::vector<std::time_t> dates;
	std::time_t date = std::time_t(nullptr);

	std::ifstream infile(filename);
	std::string line;
    int counter = 0;
    int max_iterations = 800;

    // Check if the file was opened successfully
    if (!infile.is_open()) {
        std::cerr << "Error: File could not be opened!" << std::endl;
        return std::make_tuple(0, 0);
    }

    if (std::getline(infile, line)) {
        std::istringstream iss(line);
        float value;
        
        // Process floats from the line, separating by spaces
        while (iss >> value && counter < max_iterations) {
            closes.push_back(value);        
            dates.push_back(date);          
            date += 86400;                 
            counter++;
        }
        
        // Continue adding any remaining values to testSet after max_iterations
        while (iss >> value) {
            testSet.push_back(value);       
        }
    }

    infile.close();

	auto ps = PriceSeries();
	ps.setCloses(closes);
	ps.setDates(dates);

	// Train model 
	double rmse;
	double duration;
	switch (model) {
		case 0: {
			auto start = std::chrono::high_resolution_clock::now();
			auto ar = ps.getAR(order);
			ar->forecast(200);
			auto end = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			TimeSeries<double> forecast = ar->getForecasted();

			// Get RMSE
			double se = 0;
			auto it = forecast.begin();
			for (int i = 0; i < forecast.size(); i++, it++) {
				se += pow(it->second - testSet[i], 2);
			}
			rmse = sqrt(se / 200);
			break;
		}

		case 1: {
			auto start = std::chrono::high_resolution_clock::now();
			auto ma = ps.getMA(order);
			ma->forecast(200);
			auto end = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			TimeSeries<double> forecast = ma->getForecasted();

			// Get RMSE
			double se = 0;
			auto it = forecast.begin();
			for (int i = 0; i < forecast.size(); i++, it++) {
				se += pow(it->second - testSet[i], 2);
			}
			rmse = sqrt(se / 200);
			break;
		}

		case 2: {
			auto start = std::chrono::high_resolution_clock::now();
			auto arma = ps.getARMA(order, order);
			arma->forecast(200);
			auto end = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			TimeSeries<double> forecast = arma->getForecasted();

			// Get RMSE
			double se = 0;
			auto it = forecast.begin();
			for (int i = 0; i < forecast.size(); i++, it++) {
				se += pow(it->second - testSet[i], 2);
			}
			rmse = sqrt(se / 200);
			break;
		}
	}
	return std::make_tuple(rmse, duration);
}

void testModel(const std::string& filename_base, int model, int order) {
	std::cout << "Running tests for " << filename_base << "\n";
	double sumRmse = 0;
	double sumDuration = 0;
	
	for (int i = 0; i < 5; ++i) {
		std::string filename = filename_base + std::to_string(i) + ".txt";
		auto [rmse, duration] = priceSeriesFromTxt(filename, model, order);
		sumRmse += rmse;
		sumDuration += duration;
	}

	std::cout << "Average RMSE: " << sumRmse / 5 << "\n";
	std::cout << "Average Duration: " << sumDuration / 5 << "\n\n";
}

int main() {
	testModel("ar(5)_", 0, 5);
	testModel("ar(25)_", 0, 25);
	testModel("ma(5)_", 1, 5);
	testModel("ma(25)_", 1, 25);
	testModel("arma(5,5)_", 2, 5);
	testModel("arma(25,25)_", 2, 25);

	return 0;
}