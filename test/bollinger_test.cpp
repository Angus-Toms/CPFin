#include <gtest/gtest.h>
#include "priceseries.hpp"
#include "overlays/bollinger.hpp"

class BollingerTest : public testing::Test {
protected:
    BollingerTest() {
        priceSeries = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
    }
    std::unique_ptr<PriceSeries> priceSeries;
};

TEST_F(BollingerTest, CheckLengthsSimple) {
    const auto ps = std::make_unique<PriceSeries>();
    ps->setCloses({10, 11, 12, 13, 14});
    ps->setDates({0, 1, 2, 3, 4});
    ps->setCount(5);

    EXPECT_EQ(
        ps->getBollingerBands(5, 1, MovingAverageType::SMA)->getDataMap().size(),
        1
    );
    EXPECT_EQ(
        ps->getBollingerBands(3, 1, MovingAverageType::EMA)->getDataMap().size(),
        3
    );
}

TEST_F(BollingerTest, CheckLengths) {
    const auto& longPriceSeries = PriceSeries::getPriceSeries("AAPL", "2018-01-01", "2023-12-31", "1d");
    std::vector<int> bollingerPeriods = {13, 45, 67, 173, 409};
    for (const auto& period : bollingerPeriods) {
        auto bollinger = longPriceSeries->getBollingerBands(period, 2, MovingAverageType::SMA);
        EXPECT_EQ(
            bollinger->getDataMap().size(),
            longPriceSeries->getCount() - period + 1
        );
    }
}

TEST_F(BollingerTest, InvalidArguments) {
    // Period less than 1
    EXPECT_THROW(
        priceSeries->getBollingerBands(0, 2, MovingAverageType::SMA),
        std::invalid_argument
    );

    // Period greater than number of data points
    EXPECT_THROW(
        priceSeries->getBollingerBands(31, 2, MovingAverageType::SMA),
        std::invalid_argument
    );

    // Number of standard deviations less than or equal to 0
    EXPECT_THROW(
        priceSeries->getBollingerBands(20, 0, MovingAverageType::SMA),
        std::invalid_argument
    );
}