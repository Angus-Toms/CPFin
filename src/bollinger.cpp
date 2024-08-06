#include "bollinger.hpp"
#include "priceseries.hpp"

// Constructor -----------------------------------------------------------------
BollingerBands::BollingerBands(const PriceSeries& priceSeries, int window, double numStdDev, MovingAverageType maType)
    : priceSeries(priceSeries), window(window), numStdDev(numStdDev), maType(maType) {
    // Set table printing values
    std::string maTypeStr = (maType == MovingAverageType::SMA) ? "SMA" : "EMA";
    name = fmt::format("{}: Bollinger Bands({}d, ±{:.1f}σ, {})", priceSeries.getTicker(), window, numStdDev, maTypeStr);
    columnHeaders = {"Date", "Lower Band", maTypeStr, "Upper Band"};
    columnWidths = {13, 15, 15, 15};

    calculate();
}

void BollingerBands::calculate() {
    // TODO: Debug SD calculations
    size_t windowSize = static_cast<size_t>(window);

    const std::map<std::time_t, OHCLRecord>& priceData = priceSeries.getData();
    auto wRight = priceData.begin();
    auto wLeft = priceData.begin();

    // Ensure window size is valid
    if (windowSize > priceData.size()) {
        throw std::invalid_argument("Could not get Bollinger Bands: Window size must be less than data length");
    }

    // Get std dev of first window
    double sum = 0;
    double sumSquares = 0;
    for (size_t i = 0; i < windowSize; i++) {
        double close = wRight->second.getClose();
        sum += close;
        sumSquares += close * close;

        wRight++;
    }
    wRight--;  // Move one step back to correct position after the for loop

    // Slide window, recompute std dev and bands for each
    while (wRight != priceData.end()) {
        double sma = sum / windowSize;

        // Find stdDev of window: sqrt(E[X^2] - (E[X]^2))
        double stdDev = std::sqrt((sumSquares / window) - (sma * sma));

        std::cout << "SMA: " << sma << ", stdDev: " << stdDev << std::endl;

        // Get bands 
        double lowerBand = sma - numStdDev * stdDev;
        double upperBand = sma + numStdDev * stdDev;

        data[wRight->first] = std::make_tuple(lowerBand, sma, upperBand);

        // Move to the next position before sliding the window
        wRight++;
        if (wRight == priceData.end()) break;

        // Slide window 
        double newClose = wRight->second.getClose();
        double oldClose = wLeft->second.getClose();

        sum += newClose - oldClose;
        sumSquares += newClose * newClose - oldClose * oldClose;

        wLeft++;
    }
}



// Virtual methods -------------------------------------------------------------
int BollingerBands::plot() const {
    return 0;
}
std::vector<std::vector<std::string>> BollingerBands::getTableData() const {
    std::vector<std::vector<std::string>> tableData;
    for (const auto& [date, tuple] : data) {
        tableData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", std::get<0>(tuple)),
            fmt::format("{:.2f}", std::get<1>(tuple)),
            fmt::format("{:.2f}", std::get<2>(tuple))
        });
    }
    return tableData;
}
