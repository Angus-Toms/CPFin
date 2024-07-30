#pragma once 

#ifndef PRINT_UTILS_HPP
#define PRINT_UTILS_HPP

#include <string>
#include <vector>
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

// Define colors using ANSI escape codes
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

std::string getRow(const std::vector<std::string>& row, const std::vector<int>& columnWidths, const std::vector<Justification>& justifications, const std::vector<Color>& colors);

#endif // PRINT_UTILS_HPP