#include <gtest/gtest.h>
#include "priceseries.hpp"
#include "overlays/rsi.hpp"

class RSITest : public testing::Test {
protected:
    RSITest() {
        priceSeries = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
    }
    std::unique_ptr<PriceSeries> priceSeries;
};

TEST_F(RSITest, CheckLengths) {
    const auto& longPriceSeries = PriceSeries::getPriceSeries("AAPL", "2018-01-01", "2023-12-31", "1d");
    std::vector<int> rsiPeriods = {13, 45, 67, 173, 409};
    for (const auto& period : rsiPeriods) {
        auto rsi = longPriceSeries->getRSI(period);
        EXPECT_EQ(
            rsi->getDataMap().size(),
            longPriceSeries->getCount() - period
        );
    }
}

TEST_F(RSITest, InvalidArguments) {
    // Period less than 1
    EXPECT_THROW(
        priceSeries->getRSI(0),
        std::invalid_argument
    );

    // Period greater than number of data points
    EXPECT_THROW(
        priceSeries->getRSI(31),
        std::invalid_argument
    );
}