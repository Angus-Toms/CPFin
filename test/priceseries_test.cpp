#include "gtest/gtest.h"
#include "priceseries.hpp"

// Is there a better way to collect expected values?
std::string expectedTicker = "AAPL";
int expectedCount = 20;
std::vector<double> expectedOpens = {74.059998, 74.287498, 73.447502, 74.959999, 74.290001, 76.809998, 77.650002, 77.910004, 79.175003, 77.962502, 78.397499, 79.067497, 79.297501, 79.644997, 79.480003, 80.0625, 77.514999, 78.150002, 81.112503, 80.135002};
std::vector<double> expectedHighs = {75.150002, 75.144997, 74.989998, 75.224998, 76.110001, 77.607498, 78.167503, 79.267502, 79.392502, 78.875, 78.925003, 79.684998, 79.754997, 79.997498, 79.889999, 80.832497, 77.942497, 79.599998, 81.962502, 81.022499};
std::vector<double> expectedLows = {73.797501, 74.125, 73.1875, 74.370003, 74.290001, 76.550003, 77.0625, 77.787498, 78.042503, 77.387497, 78.022499, 78.75, 79, 79.327499, 78.912498, 79.379997, 76.220001, 78.047501, 80.345001, 79.6875};
std::vector<double> expectedCloses = {75.087502, 74.357498, 74.949997, 74.597504, 75.797501, 77.407501, 77.582497, 79.239998, 78.169998, 77.834999, 78.809998, 79.682503, 79.142502, 79.425003, 79.807503, 79.577499, 77.237503, 79.422501, 81.084999, 80.967499};
std::vector<double> expectedAdjCloses = {72.876114, 72.167603, 72.74263, 72.400543, 73.565216, 75.127777, 75.297615, 76.906326, 75.867836, 75.542709, 76.488983, 77.335785, 76.811676, 77.085869, 77.457115, 77.233871, 74.962791, 77.083443, 78.696976, 78.582939};
std::vector<long> expectedVolumes = {135480400, 146322800, 118387200, 108872000, 132079200, 170108400, 140644800, 121532000, 161954400, 121923600, 108829200, 137816400, 110843200, 101832400, 104472000, 146537600, 161940000, 162234000, 216229200, 126743200};

class PriceSeriesTest : public testing::Test {
protected:
    PriceSeriesTest() {
        priceSeries = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
    }
    
    std::unique_ptr<PriceSeries> priceSeries;
};

// This is a bit weird, need to think about values are read from Yahoo downloaded file
int tolerance = 3;
double roundToTolerance(double value) {
    return std::round(value * std::pow(10, tolerance)) / std::pow(10, tolerance);
}

TEST_F(PriceSeriesTest, GetData) {
    EXPECT_EQ(priceSeries->getTicker(), "AAPL");

    auto count = priceSeries->getCount();
    EXPECT_EQ(count, expectedCount);

    const auto& opens = priceSeries->getOpens();
    const auto& highs = priceSeries->getHighs();
    const auto& lows = priceSeries->getLows();
    const auto& closes = priceSeries->getCloses();
    const auto& adjCloses = priceSeries->getAdjCloses();

    for (size_t i = 0; i < count; ++i) {
        EXPECT_EQ(
            roundToTolerance(opens[i]),
            roundToTolerance(expectedOpens[i])
        );
        EXPECT_EQ(
            roundToTolerance(highs[i]),
            roundToTolerance(expectedHighs[i])
        );
        EXPECT_EQ(
            roundToTolerance(lows[i]),
            roundToTolerance(expectedLows[i])
        );
        EXPECT_EQ(
            roundToTolerance(closes[i]),
            roundToTolerance(expectedCloses[i])
        );
        EXPECT_EQ(
            roundToTolerance(adjCloses[i]),
            roundToTolerance(expectedAdjCloses[i])
        );
    }
    EXPECT_EQ(priceSeries->getVolumes(), expectedVolumes);
}

TEST_F(PriceSeriesTest, InvalidArguments) {
    // Start date less than 0
    EXPECT_THROW(
        PriceSeries::getPriceSeries("AAPL", -100000, 100000, "1d"),
        std::invalid_argument
    );

    // Start date in future 
    EXPECT_THROW(
        PriceSeries::getPriceSeries("AAPL", std::time(nullptr) + 10000, 10000, "1d"),
        std::invalid_argument
    );

    // Start date after end date
    EXPECT_THROW(
        PriceSeries::getPriceSeries("AAPL", 10000, 9999, "1d"),
        std::invalid_argument
    );

    // Invalid interval 
    EXPECT_THROW(
        PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "xyz"),
        std::invalid_argument
    );
}

TEST_F(PriceSeriesTest, Constructors) {
    // Check time/string conversion 
    auto ps1 = PriceSeries::getPriceSeries(
        "AAPL",
        1577836800, // 2020-01-01
        1580428800, // 2020-01-31
        "1d"
    );
    const auto& opens = priceSeries->getOpens();
    const auto& highs = priceSeries->getHighs();
    const auto& lows = priceSeries->getLows();
    const auto& closes = priceSeries->getCloses();
    const auto& adjCloses = priceSeries->getAdjCloses();

    const auto& ps1Opens = ps1->getOpens();
    const auto& ps1Highs = ps1->getHighs();
    const auto& ps1Lows = ps1->getLows();
    const auto& ps1Closes = ps1->getCloses();
    const auto& ps1AdjCloses = ps1->getAdjCloses();

    
    for (size_t i = 0; i < priceSeries->getCount(); ++i) {
        EXPECT_EQ(
            roundToTolerance(opens[i]),
            roundToTolerance(ps1Opens[i])
        );
        EXPECT_EQ(
            roundToTolerance(highs[i]),
            roundToTolerance(ps1Highs[i])
        );
        EXPECT_EQ(
            roundToTolerance(lows[i]),
            roundToTolerance(ps1Lows[i])
        );
        EXPECT_EQ(
            roundToTolerance(closes[i]),
            roundToTolerance(ps1Closes[i])
        );
        EXPECT_EQ(
            roundToTolerance(adjCloses[i]),
            roundToTolerance(ps1AdjCloses[i])
        );
    }
    EXPECT_EQ(priceSeries->getVolumes(), ps1->getVolumes());
}

TEST_F(PriceSeriesTest, GetOverlays) {
    EXPECT_NO_THROW(
        priceSeries->addSMA(5);
    );
    EXPECT_NO_THROW(
        priceSeries->addEMA(5);
    );
    EXPECT_NO_THROW(
        priceSeries->addRSI(14)
    );
    EXPECT_NO_THROW(
        priceSeries->addMACD(5, 15, 5)
    );
    EXPECT_NO_THROW(
        priceSeries->addBollingerBands(20, 2)
    );
}

TEST_F(PriceSeriesTest, Outputs) {
    EXPECT_NO_THROW(
        priceSeries->toString();
    );
    EXPECT_NO_THROW(
        priceSeries->exportToCSV();
    );
}