#pragma once

namespace minotaur {
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
    // flags to reprensent the state of each wall of the cell, false means the wall is down
    //these are changeable during the game to simulate the changing of the maze
    bool north = false;
    bool south = false;
    bool east = false;
    bool west = false;
    //operator overload to allow comparison of cells
    constexpr bool operator==(const Cell& o) const { return x == o.x && y == o.y; }
};

} 
