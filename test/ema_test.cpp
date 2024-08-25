#include "gtest/gtest.h"
#include "overlays/ema.hpp"
#include "priceseries.hpp"

class EMATest : public ::testing::Test {
protected:
    EMATest() {
        priceSeries = std::make_unique<PriceSeries>();

        // Create a PriceSeries with 5 data points
        priceSeries->setCloses(closes);
        priceSeries->setDates({0, 1, 2, 3, 4});
        priceSeries->setCount(5);
    }

    std::vector<double> closes = {10.0, 12.0, 15.0, 14.0, 16.0};
    std::unique_ptr<PriceSeries> priceSeries = std::make_unique<PriceSeries>();
};

TEST_F(EMATest, CalculateEMASinglePeriod) {
    std::shared_ptr<EMA> ema = priceSeries->getEMA(1);

    // Check if calculated EMA matches expected values
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(ema->getData().at(i), closes[i]);
    }
}

TEST_F(EMATest, InvalidArguments) {
    // Period less than 1
    EXPECT_THROW(
        priceSeries->getEMA(0),
        std::invalid_argument
    );

    // Period greater than number of data points
    EXPECT_THROW(
        priceSeries->getEMA(6),
        std::invalid_argument
    );

    // Smoothing factor less than 0
    EXPECT_THROW(
        priceSeries->getEMA(2, -0.1),
        std::invalid_argument
    );

    // Smoothing factor greater than 1
    EXPECT_THROW(
        priceSeries->getEMA(2, 1.1),
        std::invalid_argument
    );
}

TEST_F(EMATest, CheckLengths) {
    const auto& longPriceSeries = PriceSeries::getPriceSeries("AAPL", "2015-01-01", "2022-12-31", "1d");
    std::vector<int> emaPeriods = {7, 26, 92, 116, 441};
    for (const auto& period : emaPeriods) {
        auto ema = longPriceSeries->getEMA(period);
        EXPECT_EQ(
            ema->getData().size(),
            longPriceSeries->getCount() - period + 1
        );
    }
}