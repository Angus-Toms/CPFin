#include "series.hpp"
#include "time_utils.hpp"

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
        std::string dateStr, openStr, highStr, lowStr, closeStr;
        std::getline(lineStream, dateStr, ',');
        std::getline(lineStream, openStr, ',');
        std::getline(lineStream, highStr, ',');
        std::getline(lineStream, lowStr, ',');
        std::getline(lineStream, closeStr, ',');
        OHCLRecord ohcl(std::stod(openStr), std::stod(highStr), std::stod(lowStr), std::stod(closeStr));
        
        std::time_t date = dateStringToEpoch(dateStr);
        data[date] = ohcl;
    }
}
    
PriceSeries::PriceSeries(const std::string& ticker, std::time_t start, std::time_t end)
    : ticker(ticker), start(start), end(end), interval("1d") {
    fetchCSV();        
}

std::string PriceSeries::toString() {
    std::ostringstream result;
    result << "| " << ticker << " |\n----------\n";
    for (auto& [date, ohcl] : data) {
        result << "| " << epochToDateString(date) << " | " << ohcl.toString() << "\n";
    }
    return result.str();
}
