#pragma once 

#ifndef PRINT_UTILS_HPP
#define PRINT_UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h>

enum class Justification {
    LEFT,
    RIGHT,
    CENTER
};

enum class Ticks {
    NONE,
    UPPER,
    LOWER,
    BOTH
};

enum class Colors {
    RED,
    GREEN,
    WHITE
};

enum class Color {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    WHITE,   // Default color
    RESET
};

// Box drawing constant characters
const std::string TL_CORNER = "┌";
const std::string TR_CORNER = "┐";
const std::string BL_CORNER = "└";
const std::string BR_CORNER = "┘";
const std::string LV_JUNCTION = "├";
const std::string RV_JUNCTION = "┤";
const std::string TH_JUNCTION = "┬";
const std::string BH_JUNCTION = "┴";
const std::string C_JUNCTION = "┼";
const std::string V_LINE = "│";
const std::string H_LINE = "─";


std::string getTopLine(const std::vector<int>& columnWidths);
std::string getMidLine(const std::vector<int>& columnWidths, Ticks ticks);
std::string getBottomLine(const std::vector<int>& columnWidths);

std::string getRow(const std::vector<std::string>& row, 
                   const std::vector<int>& columnWidths, 
                   const std::vector<Justification>& justifications, 
                   const std::vector<Color>& colors);

std::string getTable(const std::string title, 
                     const std::vector<std::time_t> dates, 
                     const std::vector<std::vector<std::string>>& data, 
                     const std::vector<std::string>& headers, 
                     const std::vector<int>& columnWidths, 
                     const std::vector<Justification>& justifications, 
                     bool highlightChange);

#endif // PRINT_UTILS_HPP