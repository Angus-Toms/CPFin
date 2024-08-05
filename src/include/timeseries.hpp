#pragma once 

#ifndef TIMESERIES_HPP
#define TIMESERIES_HPP

#include <map>
#include <ctime>
#include <vector>
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
    virtual std::vector<std::vector<std::string>> getTableData() const = 0;

    std::string toString();
};

// Templated definition must be in header file
template <typename T>
std::string TimeSeries<T>::toString() {
    int totalWidth = 0;
    int columnCount = 0;
    for (const auto& width : columnWidths) {
        totalWidth += width;
        columnCount++;
    }
    // Account for width of column seperators
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

#endif // TIMESERIES_HPP