#pragma once
#include "world/MazeShape.hpp"

namespace minotaur::world {

/**
 * @brief Build configuration for generating a MazeState
 * @author @Cole_McGregor
 * @date 2026-04-21
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

struct MazeBuildConfig
{
    // full backing grid dimensions
    int backingWidth = 0;
    int backingHeight = 0;

    // active maze footprint dimensions
    int mazeWidth = 0;
    int mazeHeight = 0;

    // shape of the active maze footprint, defaulting to rectangular
    MazeShape shape = MazeShape::Rectangle;
};

} // namespace minotaur::world