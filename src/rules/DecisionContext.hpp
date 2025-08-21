#pragma once
#include <vector>
#include "world/Cell.hpp"  // for world::Cell

namespace minotaur {

/**
 * @brief A context for the controller to make a decision
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

// Forward declarations
class Actor;
namespace world { class MazeState; }

/**
 * Read-only view the Controller uses to pick an Action.
 * different in action and purpose from the action context
 */
struct DecisionContext {
    const world::MazeState&  maze;        // map info for planning
    const Actor&             self;        // acting unit
    const Actor&             opponent;    // opposing unit
    int                      apRemaining = 0;          // AP left this turn
    std::vector<world::Cell> visible;                  // FOV cells (optional)
    void*                    rng = nullptr;            // plug in your RNG later
};

} 
