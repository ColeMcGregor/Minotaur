#pragma once

namespace minotaur {

/**
 * @brief A minimal, dependency-light context an Action needs to read/write.
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a simple struct that represents a context for an action
 */

//forward declarations
namespace world    { class MazeState; }
namespace entities { struct Position; class Inventory; struct Stats; class StatusEffects; }

/**
 * Read/write view of game state passed to Actions.
 * - MazeState  : query terrain (walls, neighbors, etc.)
 * - Position   : mutate the actor's grid cell
 * - Inventory  : check/consume items when actions use them
 * - Stats      : adjust AP or other numeric stats
 * - StatusEffects : add/remove or tick effects
 */
struct ActionContext {
    world::MazeState&        maze;
    entities::Position&      pos;
    entities::Inventory&     inv;
    entities::Stats&         stats;
    entities::StatusEffects& effects;
};

} 
