#pragma once 

#ifndef TIMESERIES_HPP
#define TIMESERIES_HPP

#include <map>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "print_utils.hpp"

template <typename T>
class TimeSeries {
protected:
    std::map<std::time_t, T> data;

    // Info required for table printing
    std::string name;
    std::vector<std::string> columnHeaders;
    std::vector<int> columnWidths;

public:
    virtual ~TimeSeries() = default;
    virtual int plot() const = 0;

    const std::map<std::time_t, T>& getData() const;

    // Table writing and exporting
    virtual std::vector<std::vector<std::string>> getTableData() const = 0;
    std::string toString();
    void exportToCSV(const std::string& filePath = "", 
                     char seperator = ',',
                     bool includeHeader = true, int precision = 2) const;
};

// Templated definition must be in header file
template <typename T>
const std::map<std::time_t, T>& TimeSeries<T>::getData() const {
    return data;
}

template <typename T>
std::string TimeSeries<T>::toString() {
    int totalWidth = 0;
    for (const auto& width : columnWidths) {
        totalWidth += width;
    }
    // Account for width of column seperators
    auto columnCount = columnWidths.size();
    totalWidth += (columnCount-1);

    // Title
    std::string table = getTopLine({totalWidth});
    table += getRow({name}, {totalWidth}, {Justification::CENTER}, {Color::WHITE});
    table += getMidLine(columnWidths, Ticks::LOWER);

    // Headings 
    std::vector<Justification> justifications(columnCount-1, Justification::RIGHT);
    justifications.insert(justifications.begin(), Justification::LEFT);
    std::vector<Color> colors(columnCount, Color::WHITE);
    table += getRow(columnHeaders, columnWidths, justifications, colors);
    table += getMidLine(columnWidths, Ticks::BOTH);

    // Conditional highlighting
    std::vector<double> lastValues(columnCount-1);
    bool isFirstRow = true;

    // Content rows
    for (const auto& row : getTableData()) {
        if (!isFirstRow) {
            for (int i = 1; i < columnCount; i++) {
                double value = std::stod(row[i]);
                double lastValue = lastValues[i-1];
                if (value > lastValue) {
                    colors[i] = Color::GREEN; 
                } else if (value < lastValue) {
                    colors[i] = Color::RED;
                } else {
                    colors[i] = Color::WHITE;
                }
            }
        } else {
            isFirstRow = false;
        }
        table += getRow(row, columnWidths, justifications, colors);

        // Update last values 
        for (int i = 1; i < columnCount; i++) {
            lastValues[i-1] = std::stod(row[i]);
        }
    }

    // Bottom line 
    table += getBottomLine(columnWidths);
    return table;
}

template <typename T>
void TimeSeries<T>::exportToCSV(const std::string& filePath, char separator, bool includeHeader, int precision) const {
    std::string path = filePath.empty() ? name + ".csv" : filePath;
    std::ofstream file(path);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open file for writing.");
    }

    // Write header if required 
    if (includeHeader) {
        size_t columnCount = columnHeaders.size();
        for (size_t i = 0; i < columnCount; ++i) {
            file << columnHeaders[i];
            if (i != columnCount-1) {
                file << separator;
            }
        }
        file << "\n";
    }

    // Set precision 
    file << std::fixed << std::setprecision(precision);

    const auto& tableData = getTableData();
    for (const auto& row : tableData) {
        if (!row.empty()) {
            file << row[0];

            // Write rest of row (can safely assume all are numeric)
            for (size_t i = 1; i < row.size(); ++i) {
                file << separator;
                file << row[i];
            }
            file << "\n";
        }
    }
}
#endif // TIMESERIES_HPP