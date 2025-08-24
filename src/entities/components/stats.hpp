#pragma once

namespace minotaur::entities::components {

/**
 * @brief A component that represents the stats of an entity
 * 
 * @author @Cole_McGregor
 * @date 2025-08-21
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a simple struct that represents the stats of an entity...
 * it is completely mutable, and can be changed during the game, by effects and the like
 */

struct Stats {
    int baseAP    = 0;   // turn start AP (before effects)
    int currentAP = 0;   // remaining AP this turn
    int maxHitpoints = 0;   // max health points
    int currentHitpoints = 0;   // current health points
    int attack    = 0;   // attack power value
    int defense   = 0;   // defense value
    int sightRange = 0;   // radius of sight before fog of war
};

} 
