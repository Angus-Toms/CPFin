#include "print_utils.hpp"
#include <iostream>

bool isNumber(const std::string& str) {
    // Check if the string is empty or consists only of whitespace
    if (str.empty() || std::all_of(str.begin(), str.end(), isspace)) {
        return false;
    }

    // Try to convert the string to a double
    try {
        std::size_t pos;
        std::stod(str, &pos);
        // Check if the entire string was converted
        return pos == str.size();
    } catch (const std::invalid_argument& e) {
        return false;
    } catch (const std::out_of_range& e) {
        return false;
    }
}

std::string colorToAnsi(Color color) {
    switch (color) {
        case Color::RED:    return "\033[31m";
        case Color::GREEN:  return "\033[32m";
        case Color::YELLOW: return "\033[33m";
        case Color::BLUE:   return "\033[34m";
        case Color::RESET:  return "\033[0m";
        default:            return "\033[37m"; // Default to white
    }
}

std::string getTopLine(const std::vector<int>& columnWidths) {
    std::string topLine = TL_CORNER;
    for (size_t i = 0; i < columnWidths.size(); ++i) {
        topLine += fmt::format("{:─>{}}", "", columnWidths[i]);
        if (i < columnWidths.size() - 1) {
            topLine += TH_JUNCTION;
        }
    }
    topLine += TR_CORNER + "\n";
    return topLine;
}

std::string getMidLine(const std::vector<int>& columnWidths, Ticks ticks) {
    std::string midLine = LV_JUNCTION;
    for (size_t i = 0; i < columnWidths.size(); i++) {

        midLine += fmt::format("{:─>{}}", "", columnWidths[i]);

        if (i < columnWidths.size() - 1) {
            switch (ticks) {
                case Ticks::BOTH:
                    midLine += C_JUNCTION;
                    break;
                case Ticks::UPPER:
                    midLine += BH_JUNCTION;
                    break;
                case Ticks::LOWER:
                    midLine += TH_JUNCTION;
                    break;
                default:
                    midLine += H_LINE;
                    break;
            }
        }
    }
    midLine += RV_JUNCTION + "\n";
    return midLine;
}

std::string getBottomLine(const std::vector<int>& columnWidths) {
    std::string bottomLine = BL_CORNER;
    for (size_t i = 0; i < columnWidths.size(); ++i) {
        bottomLine += fmt::format("{:─>{}}", "", columnWidths[i]);
        if (i < columnWidths.size() - 1) {
            bottomLine += BH_JUNCTION;
        }
    }
    bottomLine += BR_CORNER + "\n";
    return bottomLine;
}

std::string getRow(const std::vector<std::string>& row, const std::vector<int>& columnWidths, const std::vector<Justification>& justifications, const std::vector<Color>& colors) {
    auto columnCount = row.size();
    if (columnCount != columnWidths.size() || columnCount != justifications.size()) {
        throw std::invalid_argument("Column count must match column widths, justifications, and colors");
    }
    
    std::string rowStr = V_LINE;
    for (size_t i = 0; i < row.size(); i++) {

        // Convert numbers to 2dp
        std::string text = isNumber(row[i]) ?
            fmt::format("{:.2f}", std::stod(row[i])) :
            row[i];

        std::string colorCode = colorToAnsi(colors[i]);
        std::string resetCode = colorToAnsi(Color::RESET);
 
        // Correctly align content
        switch (justifications[i]) {
            case Justification::LEFT:
                rowStr += fmt::format("{}{:<{}}{}", colorCode, text, columnWidths[i], resetCode);
                break;
            case Justification::RIGHT:
                rowStr += fmt::format("{}{:>{}}{}", colorCode, text, columnWidths[i], resetCode);
                break;
            case Justification::CENTER:
                rowStr += fmt::format("{}{:^{}}{}", colorCode, text, columnWidths[i], resetCode);
                break;
        }

        if (i < row.size() - 1) {
            rowStr += V_LINE;
        }
    }
    rowStr += V_LINE + "\n";
    return rowStr;
}

// TODO: START HERE: Work out interface. Pass seperate date and data vectors? A map? TimeSeries object?
template <typename T>
std::string getTable(const std::string title, const std::map<std::time_t, T> data, const std::vector<std::string>& headers, const std::vector<int>& columnWidths, const std::vector<Justification>& justifications, bool highlightChange) {
    int columnCount = data[0].size();

    if (headers.size() < columnCount) {
        throw std::invalid_argument("Could not build table: Too few headers provided");
    } else if (headers.size() > columnCount) {
        throw std::invalid_argument("Could not build table: Too many headers provided");
    }

    if (columnWidths.size() < columnCount) {
        throw std::invalid_argument("Could not build table: Too few column widths provided");
    } else if (columnWidths.size() > columnCount) {
        throw std::invalid_argument("Could not build table: Too many column widths provided");
    }

    if (justifications.size() < columnCount) {
        throw std::invalid_argument("Could not build table: Too few justifications provided");
    } else if (justifications.size() > columnCount) {
        throw std::invalid_argument("Could not build table: Too many justifications provided");
    }

    int totalWidth = 1;
    for (int width : columnWidths) {
        totalWidth += width + 1;
    }
    std::vector<Color> colors(columnCount, Color::WHITE);

    // Build title and headers 
    std::string table = getTopLine({totalWidth});
    table += getRow({title}, {totalWidth}, {Justification::CENTER}, {Color::WHITE});
    table += getMidLine(columnWidths, Ticks::LOWER);
    table += getRow(headers, columnWidths, justifications, colors);
    table += getMidLine(columnWidths, Ticks::BOTH);

    // Build data rows
    std::vector<double> lastValues;
    for (const auto& elem : data[0]) {
        lastValues.push_back(std::stod(elem));
    }

    for (const auto& row : data) {
        if (highlightChange) {
            for (int i = 0; i < columnCount; i++) {
                double value = std::stod(row[i]);
                if (value > lastValues[i]) {
                    colors[i] = Color::GREEN;
                } else if (value < lastValues[i]) {
                    colors[i] = Color::RED;
                } else {
                    colors[i] = Color::WHITE;
                }
            }
            // Set last values to current row
            lastValues.clear();
            for (const auto& elem : row) {
                lastValues.push_back(std::stod(elem));
            }
        }
        table += getRow(row, columnWidths, justifications, colors);
    }    
    // Build bottom line
    table += getBottomLine(columnWidths);
    return table;
}