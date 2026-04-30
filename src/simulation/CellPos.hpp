#pragma once

#include <cstdint>

namespace minotaur::simulation {

/**
 * @brief Represents a position in the maze grid.
 */
struct CellPos
{
    int x = 0;
    int y = 0;

    constexpr bool operator==(const CellPos& o) const
    {
        return x == o.x && y == o.y;
    }

    constexpr bool operator!=(const CellPos& o) const
    {
        return !(*this == o);
    }
};

} // namespace minotaur::simulation