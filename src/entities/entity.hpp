#pragma once
#include <cstdint>
#include <string>

namespace minotaur {

/**
 * @brief Base type for anything placed in the game world (actors, props, traps, etc.).
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a base class for all entities
 */

class Entity {
public:
    using Id = std::uint32_t;
    virtual ~Entity() = default;

    /// Stable identifier (useful for logs, maps, and lookups).
    virtual Id id() const = 0;

    /// Human-readable label for UI/logging (e.g., "Hunted", "Minotaur", "Chest").
    virtual const std::string& label() const = 0;
};

} 
