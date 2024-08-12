#include "priceseries.hpp"

#include "averages.hpp"
#include "returns.hpp"
#include "bollinger.hpp"
#include "rsi.hpp"

// OHCLRecord struct ===========================================================
OHCLRecord::OHCLRecord(double open, double high, double low, double close, double adjClose, double volume)
    : open(open), high(high), low(low), close(close), adjClose(adjClose), volume(volume) {};

std::string OHCLRecord::toString() const {
    return "";
}

// Getters ---------------------------------------------------------------------
double OHCLRecord::getOpen() const { return open; }
double OHCLRecord::getHigh() const { return high; }
double OHCLRecord::getLow() const { return low; }
double OHCLRecord::getClose() const { return close; }
double OHCLRecord::getAdjClose() const { return adjClose; }
double OHCLRecord::getVolume() const { return volume; }

// PriceSeries class ===========================================================
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
    parseCSV(readBuffer, data);
}

void PriceSeries::parseCSV(const std::string& readBuffer, std::map<std::time_t, OHCLRecord>& data) {
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
        std::getline(lineStream, openStr, ',');
        std::getline(lineStream, highStr, ',');
        std::getline(lineStream, lowStr, ',');
        std::getline(lineStream, closeStr, ',');
        std::getline(lineStream, adjCloseStr, ',');
        std::getline(lineStream, volumeStr, ',');
        OHCLRecord ohcl(std::stod(openStr), std::stod(highStr), std::stod(lowStr), std::stod(closeStr), std::stod(adjCloseStr), std::stod(volumeStr));
        
        std::time_t date = dateStringToEpoch(dateStr);
        data[date] = ohcl;
    }
}

// Virtual methods -------------------------------------------------------------
int PriceSeries::plot() const {
    // Construct date strings
    // TODO: Implement
    return 0;
}

std::vector<std::vector<std::string>> PriceSeries::getTableData() const {
    std::vector<std::vector<std::string>> allData;
    for (const auto& [date, record] : data) {
        allData.push_back({
            epochToDateString(date),
            fmt::format("{:.2f}", record.getOpen()),
            fmt::format("{:.2f}", record.getHigh()),
            fmt::format("{:.2f}", record.getLow()),
            fmt::format("{:.2f}", record.getClose()),
            fmt::format("{:.2f}", record.getAdjClose()),
            fmt::format("{:.0f}", record.getVolume())
        });
    }
    return allData;
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
std::vector<std::time_t> PriceSeries::getDates() const {
    std::vector<std::time_t> dates;
    for (const auto& [date, _] : data) {
        dates.push_back(date);
    }
    return dates;
}
std::vector<double> PriceSeries::getOpens() const {
    std::vector<double> opens;
    for (const auto& [date, record] : data) {
        opens.push_back(record.getOpen());
    }
    return opens;
}
std::vector<double> PriceSeries::getHighs() const {
    std::vector<double> highs;
    for (const auto& [date, record] : data) {
        highs.push_back(record.getHigh());
    }
    return highs;
}
std::vector<double> PriceSeries::getLows() const {
    std::vector<double> lows;
    for (const auto& [date, record] : data) {
        lows.push_back(record.getLow());
    }
    return lows;
}
std::vector<double> PriceSeries::getCloses() const {
    std::vector<double> closes;
    for (const auto& [date, record] : data) {
        closes.push_back(record.getClose());
    }
    return closes;
}
std::vector<double> PriceSeries::getAdjCloses() const {
    std::vector<double> adjCloses;
    for (const auto& [date, record] : data) {
        adjCloses.push_back(record.getAdjClose());
    }
    return adjCloses;
}
std::vector<double> PriceSeries::getVolumes() const {
    std::vector<double> volumes;
    for (const auto& [date, record] : data) {
        volumes.push_back(record.getVolume());
    }
    return volumes;
}

// Analyses --------------------------------------------------------------------
// Simple Moving Average 
// Default window is 20d
std::unique_ptr<SMA> PriceSeries::getSMA(int window) const {
    return std::make_unique<SMA>(*this, window);
}
// Exponential Moving Average
std::unique_ptr<EMA> PriceSeries::getEMA(int window, double smoothingFactor) const {
    return std::make_unique<EMA>(*this, window, smoothingFactor);
}
// Returns
std::unique_ptr<ReturnMetrics> PriceSeries::getReturns() const {
    return std::make_unique<ReturnMetrics>(*this);
}
// Bollinger Bands
std::unique_ptr<BollingerBands> PriceSeries::getBollingerBands(int window, double numStdDev, MovingAverageType maType) const {
    return std::make_unique<BollingerBands>(*this, window, numStdDev, maType);
}
// Moving-Average Convergence/Divergence
std::unique_ptr<MACD> PriceSeries::getMACD(int aPeriod, int bPeriod, int cPeriod) const {
    return std::make_unique<MACD>(*this, aPeriod, bPeriod, cPeriod);
}
std::unique_ptr<RSI> PriceSeries::getRSI(int period) {
    return std::make_unique<RSI>(*this, period);
}

double PriceSeries::getStdDev() const {
    // Welford's Method
    double mean = 0.0;
    double M2 = 0.0;
    size_t n = 0;

    for (const auto& close : getCloses()) {
        n++;
        double delta = close - mean;
        mean += delta / n;
        double delta2 = close - mean;
        M2 += delta * delta2;
    }

    return std::sqrt(M2 / n);
}