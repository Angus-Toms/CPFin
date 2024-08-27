#include <gtest/gtest.h>
#include "priceseries.hpp"
#include "overlays/macd.hpp"

class MACDTest : public testing::Test {
protected:
    MACDTest() {
        priceSeries = PriceSeries::getPriceSeries("AAPL", "2020-01-01", "2020-01-31", "1d");
    }
    std::unique_ptr<PriceSeries> priceSeries;
};

// TODO: MACD output is incorrect
// TEST_F(MACDTest, CheckLengths) {
//     const auto& longPriceSeries = PriceSeries::getPriceSeries("AAPL", "2018-01-01", "2023-12-31", "1d");
//     std::vector<int> macdShortPeriods = {12, 26};
//     std::vector<int> macdLongPeriods = {21, 38};
//     int signalPeriod = 5;
//     for (const auto& shortPeriod : macdShortPeriods) {
//         for (const auto& longPeriod : macdLongPeriods) {
//             auto macd = longPriceSeries->getMACD(shortPeriod, longPeriod, signalPeriod);
//             EXPECT_EQ(
//                 macd->getDataMap().size(),
//                 longPriceSeries->getCount() - longPeriod - signalPeriod + 1
//             );
//         }
//     }
// }

TEST_F(MACDTest, InvalidArguments) {
    // Short period less than 1
    EXPECT_THROW(
        priceSeries->getMACD(0, 26, 9),
        std::invalid_argument
    );

    // Long period less than 1
    EXPECT_THROW(
        priceSeries->getMACD(12, 0, 9),
        std::invalid_argument
    );

    // Signal period less than 1
    EXPECT_THROW(
        priceSeries->getMACD(12, 26, 0),
        std::invalid_argument
    );

    // Short period greater than number of data points
    EXPECT_THROW(
        priceSeries->getMACD(31, 26, 9),
        std::invalid_argument
    );

    // Long period greater than number of data points
    EXPECT_THROW(
        priceSeries->getMACD(12, 31, 9),
        std::invalid_argument
    );

    // Signal period greater than number of data points
    EXPECT_THROW(
        priceSeries->getMACD(12, 26, 31),
        std::invalid_argument
    );
} 