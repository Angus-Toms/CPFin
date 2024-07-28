#include "priceseries.hpp"
#include "time_utils.hpp"

// OHCLRecord struct ===========================================================
OHCLRecord::OHCLRecord(double open, double high, double low, double close, double adjClose, double volume)
    : open(open), high(high), low(low), close(close), adjClose(adjClose), volume(volume) {};

std::string OHCLRecord::toString() const {
    std::ostringstream result;
    result << "| " << open 
        << " | " << high 
        << " | " << low
        << " | " << close
        << " | " << adjClose
        << " | " << volume << " |";
    return result.str();
};

double OHCLRecord::getClose() const {
    return close;
};

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
        throw std::invalid_argument("Could not get PriceSeries. Interval " + interval + " is not supported\n Supported intervals: " + supportedIntervals.str());
    }

    return;
}

void PriceSeries::fetchCSV() {

    // Call construction
    std::ostringstream urlBuilder;
    urlBuilder << "https://query1.finance.yahoo.com/v7/finance/download/" << ticker
                << "?period1=" << start
                << "&period2=" << end
                << "&interval=" << interval;
    std::string url = urlBuilder.str();
    std::cout << "Fetching from " << url << std::endl;

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
void PriceSeries::plot() const {
    std::cout << "TODO: Plot PriceSeries\n";
    return;
}

// Factory methods -------------------------------------------------------------
// All-argument constructor (date objects)
PriceSeries PriceSeries::getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end, const std::string& interval) {
    return PriceSeries(ticker, start, end, interval);
}

// All-argument constructor (date strings)
PriceSeries PriceSeries::getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end, const std::string& interval) {
    std::time_t startEpoch = dateStringToEpoch(start);
    std::time_t endEpoch = dateStringToEpoch(end);
    return PriceSeries(ticker, startEpoch, endEpoch, interval);
}

// No interval constructor (date objects) - defaults to interval of "1d"
PriceSeries PriceSeries::getPriceSeries(const std::string& ticker, const std::time_t start, const std::time_t end) {
    return PriceSeries(ticker, start, end, "1d");
}

// No interval constructor (date strings) - defaults to interval of "1d"
PriceSeries PriceSeries::getPriceSeries(const std::string& ticker, const std::string& start, const std::string& end) {
    std::time_t startEpoch = dateStringToEpoch(start);
    std::time_t endEpoch = dateStringToEpoch(end);
    return PriceSeries(ticker, startEpoch, endEpoch, "1d");
}

// Number of datapoints constructor (date object)
// TODO: Update to account for weekends
PriceSeries PriceSeries::getPriceSeries(const std::string& ticker, const std::time_t start, const std::string& interval, const std::size_t count) {
    std::time_t end = start + count * intervalToSeconds(interval);
    return PriceSeries(ticker, start, end, interval);
}

// Number of datapoints constructor (date string)
// TODO: Update to account for weekends
PriceSeries PriceSeries::getPriceSeries(const std::string& ticker, const std::string& start, const std::string& interval, const std::size_t count) {
    std::time_t startEpoch = dateStringToEpoch(start);
    std::time_t end = startEpoch + count * intervalToSeconds(interval);
    return PriceSeries(ticker, startEpoch, end, interval);
}

std::string PriceSeries::toString() const {
    std::ostringstream result;
    result << "| " << ticker << " |\n----------\n";
    for (auto& [date, ohcl] : data) {
        result << "| " << epochToDateString(date) << " | " << ohcl.toString() << "\n";
    }
    return result.str();

}

// Getters ---------------------------------------------------------------------
std::map<std::time_t, OHCLRecord> PriceSeries::getData() const {
    return data;
}

OHCLRecord PriceSeries::getRecord(const std::time_t date) const {
    return data.at(date);
}