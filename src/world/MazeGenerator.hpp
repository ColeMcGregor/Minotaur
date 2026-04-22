#pragma once
#include "world/MazeBuildConfig.hpp"
#include "world/MazeState.hpp"

namespace minotaur::world {

/**
 * @brief Generates MazeState instances from build configuration
 * @author @Cole_McGregor
 * @date 2026-04-21
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

class MazeGenerator
{
public:
    static MazeState generate(const MazeBuildConfig& config);

private:
    static void validateConfig(const MazeBuildConfig& config);
    static void formShape(MazeState& maze, const MazeBuildConfig& config);
    static void formRectangle(MazeState& maze, const MazeBuildConfig& config);
    static void formEllipse(MazeState& maze, const MazeBuildConfig& config);
    static void initializeClosedWalls(MazeState& maze);
    static void applyWallAlgorithm(MazeState& maze, const MazeBuildConfig& config);
};

} // namespace minotaur::world