#include <gtest/gtest.h>
#include "priceseries.hpp"
#include "overlays/sma.hpp"

class SMATest : public testing::Test {
protected:
    SMATest() {
        priceSeries = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
        priceSeries->setCloses({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        priceSeries->setDates({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        priceSeries->setCount(10);
        sma1 = priceSeries->getSMA(1);
        sma5 = priceSeries->getSMA(5);
    }

    std::unique_ptr<PriceSeries> priceSeries;
    std::shared_ptr<SMA> sma1;
    std::shared_ptr<SMA> sma5;
};

TEST_F(SMATest, TestFunctionality) {
    const auto& sma1Data = sma1->getData();
    auto it = sma1Data.begin();
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(it->second, i+1);
        it++;
    }

    const auto& sma5Data = sma5->getData();
    auto it2 = sma5Data.begin();
    for (size_t i = 4; i < 10; ++i) {
        EXPECT_EQ(it2->second, i-1);
        it2++;
    }
}

TEST_F(SMATest, CheckLengths) {
    const auto& longPriceSeries = PriceSeries::getPriceSeries("AAPL", "2018-01-01", "2023-12-31", "1d");
    std::vector<int> smaPeriods = {7, 26, 92, 116, 441};
    for (const auto& period : smaPeriods) {
        auto sma = longPriceSeries->getSMA(period);
        EXPECT_EQ(
            sma->getData().size(),
            longPriceSeries->getCount() - period + 1
        );
    }
}

TEST_F(SMATest, InvalidArguments) {
    // Period less than 1
    EXPECT_THROW(
        priceSeries->getSMA(0),
        std::invalid_argument
    );

    // Period greater than number of data points
    EXPECT_THROW(
        priceSeries->getSMA(11),
        std::invalid_argument
    );
}