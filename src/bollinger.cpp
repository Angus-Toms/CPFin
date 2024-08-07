#include "bollinger.hpp"
#include "averages.hpp"
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
    // TODO: This can greatly be improved, particularly for SMA middle band 
    // See: https://en.wikipedia.org/wiki/Standard_deviation#Rapid_calculation_methods
    std::vector<double> closes = priceSeries.getCloses();
    const std::map<std::time_t, double> movingAverages = 
        (maType == MovingAverageType::SMA) ?
        priceSeries.getSMA(window).getData() :
        priceSeries.getEMA(window).getData();

    int wStart = 0;

    for (const auto& [date, ma] : movingAverages) {
        // Calculate standard deviation for window ending at date 
        double sum = 0;
        double sumSquares = 0;
        for (int i = 0; i < window; i++) {
            double close = closes[i+wStart];
            sum += close;
            sumSquares += close * close;
        }
        double stdDev = std::sqrt((sumSquares/window) - ((sum/window) * (sum/window)));
        
        double upperBand = ma + numStdDev * stdDev;
        double lowerBand = ma - numStdDev * stdDev;

        data[date] = std::make_tuple(lowerBand, ma, upperBand);
        wStart++;
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
