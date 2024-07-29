#include "print_utils.hpp"

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

std::string getMidLine(const std::vector<int>& columnWidths, bool upperTicks, bool lowerTicks) {
    std::string midLine = LV_JUNCTION;
    for (size_t i = 0; i < columnWidths.size(); i++) {

        midLine += fmt::format("{:─>{}}", "", columnWidths[i]);

        if (i < columnWidths.size() - 1) {
            if (upperTicks && lowerTicks) {
                midLine += C_JUNCTION;
            } else if (upperTicks) {
                midLine += BH_JUNCTION;
            } else if (lowerTicks) {
                midLine += TH_JUNCTION;
            } else {
                midLine += H_LINE;
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

// Justifications: 0 = left, 1 = right
// TODO: This is messy, enums?
std::string getRow(const std::vector<std::string>& row, const std::vector<int>& columnWidths, const std::vector<bool>& justifications) {
    std::string rowStr = V_LINE;
    for (size_t i = 0; i < row.size(); i++) {

        // Correctly align content
        if (justifications[i]) {
            rowStr += fmt::format("{:>{}}", row[i], columnWidths[i]);
        } else {
            rowStr += fmt::format("{:<{}}", row[i], columnWidths[i]);
        }

        if (i < row.size() - 1) {
            rowStr += V_LINE;
        }
    }
    rowStr += V_LINE + "\n";
    return rowStr;
}