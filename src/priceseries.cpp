#include "priceseries.hpp"

#include "overlays/overlays.hpp"
#include "overlays/bollinger.hpp"
#include "overlays/ema.hpp"
#include "overlays/macd.hpp"
#include "overlays/rsi.hpp"
#include "overlays/sma.hpp"

PriceSeries::PriceSeries() = default;
PriceSeries::~PriceSeries() = default;
PriceSeries::PriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval)
    : ticker(ticker), start(start), end(end), interval(interval) {
    checkArguments();
    fetchCSV();
}

void PriceSeries::checkArguments() {
    // TODO: Validate ticker

    // Validate start time 
    if (start < 0) {
        throw std::invalid_argument("Could not get PriceSeries. Invalid start time");
    }

    if (start > std::time(nullptr)) {
        throw std::invalid_argument("Could not get PriceSeries. Start time is in the future");
    }

    if (start > end) {
        throw std::invalid_argument("Could not get PriceSeries. Start time is after end time");
    }

    // Validate end time
    if (end < 0) {
        throw std::invalid_argument("Could not get PriceSeries. Invalid end time");
    }

    if (end > std::time(nullptr)) {
        std::cout << "WARNING! End time is in future, cropping to current time\n";
        end = std::time_t(nullptr);
    }

    // Valid interval check
    if (isInvalidInterval(interval)) {
        std::ostringstream supportedIntervals;
        for (const auto& interval : VALID_INTERVALS) {
            supportedIntervals << interval << " ";
        }
        throw std::invalid_argument("Could not get PriceSeries. Interval " + interval + " is not supported\nSupported intervals: " + supportedIntervals.str());
    }
}

void PriceSeries::fetchCSV() {
    // Call construction
    std::ostringstream urlBuilder;
    urlBuilder << "https://query1.finance.yahoo.com/v7/finance/download/" << ticker
                << "?period1=" << start
                << "&period2=" << end
                << "&interval=" << interval;
    std::string url = urlBuilder.str();

    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch " << ticker << ": " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    parseCSV(readBuffer);
}

void PriceSeries::parseCSV(const std::string& readBuffer) {
    std::istringstream ss(readBuffer);
    std::string line;

    bool isFirstLine = true;
    while (std::getline(ss, line)) {
        // Skip header line
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        std::istringstream lineStream(line);
        std::string dateStr, openStr, highStr, lowStr, closeStr, adjCloseStr, volumeStr;
        std::getline(lineStream, dateStr, ',');
        dates.push_back(dateStringToEpoch(dateStr));
        std::getline(lineStream, openStr, ',');
        opens.push_back(std::stod(openStr));
        std::getline(lineStream, highStr, ',');
        highs.push_back(std::stod(highStr));
        std::getline(lineStream, lowStr, ',');
        lows.push_back(std::stod(lowStr));
        std::getline(lineStream, closeStr, ',');
        closes.push_back(std::stod(closeStr));
        std::getline(lineStream, adjCloseStr, ',');
        adjCloses.push_back(std::stod(adjCloseStr));
        std::getline(lineStream, volumeStr, ',');
        volumes.push_back(std::stol(volumeStr));
    }
}

void PriceSeries::plot() const {
    // Plot basic line 
    namespace plt = matplotlibcpp;

    std::vector<double> closes = getCloses();
    std::vector<std::time_t> xs = getDates();
    plt::named_plot("Price", xs, closes, "b-");

    // Plot overlays
    for (const auto& overlay : overlays) {
        overlay->plot();
    }

    // General plot aesthetics 
    plt::title(ticker);
    plt::legend();

    plt::show();
}

std::string PriceSeries::toString() const {
    return ticker;
}

// Factory methods -------------------------------------------------------------
std::unique_ptr<PriceSeries> PriceSeries::getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval) {
    return std::unique_ptr<PriceSeries>(new PriceSeries(ticker, start, end, interval));
}

std::unique_ptr<PriceSeries> PriceSeries::getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end, const std::string& interval) {
    return getPriceSeries(ticker, dateStringToEpoch(start), dateStringToEpoch(end), interval);
}

std::unique_ptr<PriceSeries> PriceSeries::getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end) {
    return getPriceSeries(ticker, start, end, "1d");
}

std::unique_ptr<PriceSeries> PriceSeries::getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end) {
    return getPriceSeries(ticker, dateStringToEpoch(start), dateStringToEpoch(end), "1d");
}

std::unique_ptr<PriceSeries> PriceSeries::getPriceSeries(const std::string& ticker, const std::time_t start, const std::string& interval, const std::size_t count) {
    std::time_t end = start + count * intervalToSeconds(interval);
    return getPriceSeries(ticker, start, end, interval);
}

std::unique_ptr<PriceSeries> PriceSeries::getPriceSeries(const std::string& ticker, const std::string& start, const std::string& interval, const std::size_t count) {
    std::time_t startTime = dateStringToEpoch(start);
    std::time_t end = startTime + count * intervalToSeconds(interval);
    return getPriceSeries(ticker, startTime, end, interval);
}

// Getters ---------------------------------------------------------------------
std::string PriceSeries::getTicker() const { return ticker; }
std::vector<std::time_t> PriceSeries::getDates() const { return dates; }
std::vector<double> PriceSeries::getOpens() const { return opens; }
std::vector<double> PriceSeries::getHighs() const { return highs;}
std::vector<double> PriceSeries::getLows() const { return lows; }
std::vector<double> PriceSeries::getCloses() const { return closes; }
std::vector<double> PriceSeries::getAdjCloses() const { return adjCloses; }
std::vector<long> PriceSeries::getVolumes() const { return volumes;}

// Overlays --------------------------------------------------------------------
void PriceSeries::addOverlay(std::shared_ptr<IOverlay> overlay) {
    overlays.push_back(std::move(overlay));
}

const std::vector<std::shared_ptr<IOverlay>>& PriceSeries::getOverlays() const {
    return overlays;
}

void PriceSeries::addSMA(int period) {
    addOverlay(std::make_unique<SMA>(std::make_shared<PriceSeries>(*this), period));
}

void PriceSeries::addEMA(int period, double smoothingFactor) {
    addOverlay(std::make_unique<EMA>(std::make_shared<PriceSeries>(*this), period, smoothingFactor));
}

void PriceSeries::addMACD(int aPeriod, int bPeriod, int cPeriod) {
    addOverlay(std::make_unique<MACD>(std::make_shared<PriceSeries>(*this), aPeriod, bPeriod, cPeriod));
}

void PriceSeries::addBollingerBands(int period, double numStdDev, const std::string& maType) {
    addOverlay(std::make_unique<BollingerBands>(std::make_shared<PriceSeries>(*this), period, numStdDev, maType));
}

void PriceSeries::addRSI(int period) {
    addOverlay(std::make_unique<RSI>(std::make_shared<PriceSeries>(*this), period));
}