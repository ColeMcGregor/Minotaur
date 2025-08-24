#pragma once
#include "world/Cell.hpp"

/**
 * @brief A component that represents the position of an entity in the world
 * 
 * @author @Cole_McGregor
 * @date 2025-08-21
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a simple struct that represents the position of an entity in the world, it is very simple, dont overcomplicate it
 */

namespace minotaur::entities::components {

struct Position {
    Cell cell{};
};

} 
