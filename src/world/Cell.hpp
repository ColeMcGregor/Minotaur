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

 /**
  * @brief A neighbor of a cell
  * @author @Cole_McGregor
  * @note this is small struct to hold coordinates of any cell neighbors
  */
struct Neighbor
{
    int x;
    int y;
    //direction of the neighbor relative to the cell (northerly neighbor is 0, etc)
    int direction;
    bool valid;
};


struct Cell {
    int x = 0;                       // x coordinate
    int y = 0;                       // y coordinate
    // flags to reprensent the state of each wall of the cell, false means the wall is down
    //these are changeable during the game to simulate the changing of the maze
    bool north = false;
    bool south = false;
    bool east = false;
    bool west = false;

    //list of neighbors
    Neighbor nbr[4];   //to cache the neighbors of the cell, makes math easier
    //count of neighbors, will hold how many neighbors are present
    int      nbrCount; //to be used for some algorithms
    
    //operator overload to allow comparison of cells
    constexpr bool operator==(const Cell& o) const { return x == o.x && y == o.y; }
};

} 
