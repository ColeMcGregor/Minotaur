#pragma once

#include "world/MazeBuildConfig.hpp"

namespace minotaur::simulation {

/**
 * @brief Configuration for building a simulation state.
 */
struct SimBuildConfig
{
    world::MazeBuildConfig maze;

    // exit placement
    int minExitManhattanDistance = 4;
    bool forbidExitSameAxis = true;

    // minotaur placement
    int minMinotaurDistanceFromPlayer = 8;
    int minMinotaurDistanceFromExit = 4;

    // counts
    int chestCount = 3;
    int enemyCount = 5;
};

} // namespace minotaur::simulation