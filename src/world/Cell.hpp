#pragma once

namespace minotaur::world {
/**
 * @brief A cell in the world
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a simple struct that represents a cell in the world
 */
struct Cell {
    int x = 0;                       // x coordinate
    int y = 0;                       // y coordinate
    //operator overload to allow comparison of cells
    constexpr bool operator==(const Cell& o) const { return x == o.x && y == o.y; }
};

} 
