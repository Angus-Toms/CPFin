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


std::string getRow(const std::vector<std::string>& row, const std::vector<int>& columnWidths, const std::vector<Justification>& justifications) {
    auto columnCount = row.size();
    if (columnCount != columnWidths.size() || columnCount != justifications.size()) {
        throw std::invalid_argument("Column count must match column widths and justifications");
    }
    
    std::string rowStr = V_LINE;
    for (size_t i = 0; i < row.size(); i++) {

        // Convert numbers to 2dp
        std::string text = isNumber(row[i]) ?
            fmt::format("{:.2f}", std::stod(row[i])) :
            row[i];
 
        // Correctly align content
        switch (justifications[i]) {
            case Justification::LEFT:
                rowStr += fmt::format("{:<{}}", text, columnWidths[i]);
                break;
            case Justification::RIGHT:
                rowStr += fmt::format("{:>{}}", text, columnWidths[i]);
                break;
            case Justification::CENTER:
                rowStr += fmt::format("{:^{}}", text, columnWidths[i]);
                break;
        }

        if (i < row.size() - 1) {
            rowStr += V_LINE;
        }
    }
    rowStr += V_LINE + "\n";
    return rowStr;
}