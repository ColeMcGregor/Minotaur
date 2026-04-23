#pragma once

namespace minotaur::world {

/**
 * @brief Available maze carving algorithms
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

enum class MazeAlgorithm
{
    None,

    // Test patterns (for validating wall carving + rendering)
    TestEasy,
    TestMed,
    TestHard,

    // Real algorithms (to be implemented next)
    DepthFirst,
    Prim,
    Kruskal
};

} // namespace minotaur::world