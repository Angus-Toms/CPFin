#include "overlays/ioverlay.hpp"

const std::string IOverlay::toString() const {
    return getTable(
        name,
        getTableData(),
        columnWidths,
        columnHeaders
    );
}