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

std::string getRow(const std::vector<std::string>& row, 
                   const std::vector<int>& columnWidths,
                   const std::vector<Justification>& justifications, 
                   const std::vector<Color>& colors) {
    if (row.size() != columnWidths.size() || row.size() != justifications.size()) {
        throw std::invalid_argument("Column count must match column widths, justifications, and colors");
    }
    
    std::string rowStr = V_LINE;
    for (size_t i = 0; i < row.size(); i++) {
        std::string colorCode = colorToAnsi(colors[i]);
        std::string resetCode = colorToAnsi(Color::RESET);
 
        switch (justifications[i]) {
            case Justification::LEFT:
                rowStr += fmt::format("{}{:<{}}{}", colorCode, row[i], columnWidths[i], resetCode);
                break;
            case Justification::RIGHT:
                rowStr += fmt::format("{}{:>{}}{}", colorCode, row[i], columnWidths[i], resetCode);
                break;
            case Justification::CENTER:
                rowStr += fmt::format("{}{:^{}}{}", colorCode, row[i], columnWidths[i], resetCode);
                break;
        }

        if (i < row.size() - 1) {
            rowStr += V_LINE;
        }
    }
    rowStr += V_LINE + "\n";
    return rowStr;
} 

std::string getTable(const std::string& title,
                     const std::vector<std::vector<std::string>>& tableData,
                     const std::vector<int>& columnWidths,
                     const std::vector<std::string>& columnHeaders,
                     bool changeHighlighting) {
    std::string table;
    size_t colCount = columnWidths.size();
    std::vector<Color> colors(colCount, Color::WHITE);
    std::vector<Justification> justifications = {Justification::LEFT};
    int totalWidth = colCount - 1;
    for (size_t i = 0; i < colCount; ++i) {
        totalWidth += columnWidths[i];
        if (i != 0) {
            justifications.push_back(Justification::RIGHT);
        }
    }

    table += getTopLine({totalWidth});
    table += getRow({title}, {totalWidth}, {Justification::CENTER}, {Color::WHITE}); // Title
    table += getMidLine(columnWidths, Ticks::LOWER);
    table += getRow(columnHeaders, columnWidths, justifications, colors); // Headers
    table += getMidLine(columnWidths, Ticks::BOTH);

    // Conditionally update column colors based on whether value is (in/de)creasing
    std::vector<double> previous;
    if (changeHighlighting) {
        for (size_t i = 1; i < colCount; ++i) {
            // Find first entry in each column 
            int j = 0;
            while (!isNumber(tableData[j][i])) { j++; }
            previous.push_back(std::stod(tableData[j][i]));
        }
    }
    for (const auto& row : tableData) {
        if (changeHighlighting) {
            for (size_t i = 1; i < colCount; ++i) {
                double current = 0.0;
                if (isNumber(row[i])) {
                    current = std::stod(row[i]);

                    // Compare and set color
                    if (current > previous[i - 1]) {
                        colors[i] = Color::GREEN;
                    } else if (current < previous[i - 1]) {
                        colors[i] = Color::RED;
                    } else {
                        colors[i] = Color::WHITE;
                    }

                    // Save current value
                    previous[i-1] = current;
                }
            }
        }
        table += getRow(row, columnWidths, justifications, colors);
    }
    table += getBottomLine(columnWidths);
    return table;
}