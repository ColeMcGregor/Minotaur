#pragma once
#include <vector>
#include "world/Cell.hpp"

namespace minotaur::world {

/**
 * @brief Represents the state of the maze, including backing grid and cells
 * @author @Cole_McGregor
 * @date 2026-04-21
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

class MazeState
{
public:
    // constructors
    MazeState(int width, int height);

    // dimensions
    int width() const;
    int height() const;

    // bounds and existence
    bool inBounds(int x, int y) const;
    bool hasCell(int x, int y) const;

    // access
    Cell& cellAt(int x, int y);
    const Cell& cellAt(int x, int y) const;

    // adjacency
    void finalizeNeighbors();

    // wall operations
    void openWall(int x, int y, Direction d);
    void closeWall(int x, int y, Direction d);
    void toggleWall(int x, int y, Direction d);

    // utility
    static Direction opposite(Direction d);

private:
    int width_ = 0;
    int height_ = 0;

    std::vector<Cell> grid_;

    int index(int x, int y) const;
};

} // namespace minotaur::world