#pragma once

#include <string>

#include "world/MazeStep.hpp"

namespace minotaur::world {

/**
 * @brief Prints MazeStep data in a consistent debug format
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

class MazeStepPrinter
{
public:
    static std::string toString(MazeStepType type);
    static void printStep(const MazeStep& step);
};

} // namespace minotaur::world