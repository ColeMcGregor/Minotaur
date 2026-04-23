#pragma once

#include <string>

namespace minotaur::world {

/**
 * @brief Categories of recorded maze-carving steps
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

enum class MazeStepType
{
    Start,
    Visit,
    Consider,
    Accept,
    Reject,
    Backtrack,
    Complete
};

/**
 * @brief Structured information for a single maze-carving step
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

struct MazeStep
{
    int stepNumber = 0;
    MazeStepType type = MazeStepType::Start;

    int x = -1;
    int y = -1;

    int x2 = -1;
    int y2 = -1;

    std::string candidates;
    std::string decision;
    std::string mutation;
    std::string result;
};

} // namespace minotaur::world