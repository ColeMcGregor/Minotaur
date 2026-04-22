#pragma once

namespace minotaur::world {

/**
 * @brief Cardinal directions for walls and adjacency.
 *
 * @author @Cole_McGregor
 * @date 2026-04-21
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */
enum class Direction
{
    North = 0,
    East  = 1,
    South = 2,
    West  = 3
};

/**
 * @brief A cached neighbor entry for a cell.
 *
 * Stores the coordinates of an adjacent active cell, plus the direction of that
 * neighbor relative to the owning cell.
 */
struct Neighbor
{
    int x = -1;
    int y = -1;
    Direction direction = Direction::North;
    bool valid = false;
};

/**
 * @brief A cell in the world backing grid.
 *
 * A cell may exist in the backing grid but be inactive, meaning it is not part
 * of the current maze footprint.
 */
struct Cell
{
    int x = 0;
    int y = 0;

    // Whether this cell is part of the current maze footprint.
    bool active = false;

    // Wall flags. true means a wall is present.
    bool north = false;
    bool south = false;
    bool east  = false;
    bool west  = false;

    // Cached adjacency to active neighboring cells only.
    Neighbor nbr[4] = {};
    int nbrCount = 0;

    constexpr bool operator==(const Cell& o) const
    {
        return x == o.x && y == o.y;
    }
};

} // namespace minotaur::world