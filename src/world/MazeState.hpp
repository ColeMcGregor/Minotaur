#pragma once
#include <vector>
#include <initializer_list>
#include "world/Cell.hpp"


namespace minotaur {
namespace world {

/**
 * @brief A class that represents the state of the maze, also holds the cells of the maze
 * @author @Cole_McGregor
 * @date 2025-09-03
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note also holds the direction enum for the walls and adjacency
 */

// Cardinal directions for walls/adjacency.
enum class Direction
{
    North,
    East,
    South,
    West
};

class MazeState
{
public:
//constructors
    MazeState();

    /**
     * @brief Build from a spec: one entry returns N x N square, two entries returns rectangle, three entries and on returns ragged "blob"
     * @param spec
     * @return MazeState
     */
    static MazeState generate(const std::vector<int>& spec); //make from a vector of ints
    static MazeState generate(std::initializer_list<int> spec); //make from a initializer list of ints

    // Dimensions (ragged-aware).
    int rows() const;
    int colsAtRow(int row) const;

    // Existence and access.
    bool hasCell(int x, int y) const;
    Cell& cellAt(int x, int y);
    const Cell& cellAt(int x, int y) const;

    // Compute Cell::nbr[] and nbrCount for all cells (call after construction or topology changes).
    void finalizeNeighbors();

    // Symmetric wall operations (update both sides if neighbor exists).
    void link(int x, int y, Direction d);        // remove wall both sides
    void unlink(int x, int y, Direction d);      // add wall both sides
    void toggleWall(int x, int y, Direction d);  // flip both sides

    // Utility.
    static Direction opposite(Direction d);

private:
    std::vector<std::vector<Cell>> grid_; // ragged: row i may have different length

    bool inBoundsRow(int row) const;
    bool inBounds(int x, int y) const;
};

} 
} 