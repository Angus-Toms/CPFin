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

    std::cout << "Fetching data from: " << url << "\n";

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
    // Parse CSV
    parseCSVData(readBuffer, data);
}

void PriceSeries::parseCSVData(const std::string& readBuffer, std::vector<OHCLRecord>& data) {
    
}
    
PriceSeries::PriceSeries(const std::string& ticker, std::time_t start, std::time_t end)
    : ticker(ticker), start(start), end(end), interval("1d") {
    fetchData();        
}

std::string PriceSeries::toString() {
    std::ostringstream result;
    result << "|| " << ticker << " ||\n----------\n";
    for (const OHCLRecord& record : data) {
        result << record.toString() << "\n";
    }
    return result.str();
}