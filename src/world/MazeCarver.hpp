#pragma once

#include "world/MazeAlgorithm.hpp"
#include "world/MazeState.hpp"

namespace minotaur::world {

/**
 * @brief Applies a selected maze carving algorithm to a prepared MazeState
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

class MazeCarver
{
public:
    static void carve(MazeState& maze, MazeAlgorithm algorithm);

private:
    static void carveNone(MazeState& maze);
    static void carveDepthFirst(MazeState& maze);
};

} // namespace minotaur::world