#pragma once
#include <cstdint>

/**
 * @brief The current turn of the game, belonging to the hunted or the minotaur
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @license MIT
 * @note This is a simple enum class used for understandable but fast code
 *  */
namespace minotaur {

enum class Side : std::uint8_t {
    Hunted = 0,
    Minotaur = 1
};

} 
