#pragma once

#include <vector>

#include "world/Cell.hpp"
#include "world/MazeAlgorithm.hpp"
#include "world/MazeState.hpp"
#include "world/MazeStep.hpp"

namespace minotaur::world {

/**
 * @brief A single wall-opening action used by carving routines
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */
struct CarveAction
{
    int x = -1;
    int y = -1;
    Direction dir = Direction::North;
};

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
    static void carve(MazeState& maze, MazeAlgorithm algorithm, std::vector<MazeStep>& steps);

private:
    //does nothing, voids out
    static void carveNone(MazeState& maze, std::vector<MazeStep>& steps);
    //used for basic testing of maze=UI functions
    static void carveTestEasy(MazeState& maze, std::vector<MazeStep>& steps);
    static void carveTestMed(MazeState& maze, std::vector<MazeStep>& steps);
    static void carveTestHard(MazeState& maze, std::vector<MazeStep>& steps);
    //True Maze algorithms
    static void carveDepthFirst(MazeState& maze, std::vector<MazeStep>& steps);
    static void carvePrim(MazeState& maze, std::vector<MazeStep>& steps);

    static void applyAction(MazeState& maze, const CarveAction& action, int& stepNumber, std::vector<MazeStep>& steps);
};

} // namespace minotaur::world