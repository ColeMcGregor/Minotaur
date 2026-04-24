#pragma once
#include <cstdint>
#include <string>

namespace minotaur {

/**
 * @brief Base type for anything placed in the game world
 * 
 * @author @Cole_McGregor
 * @date 2026-04-22
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

enum class EntityKind
{
    Player,
    Staircase,
    Minotaur,
    Enemy,
    Torch,
    Chest,
    SpecialWall,
    Other
};

class Entity
{
public:
    using Id = std::uint32_t;
    virtual ~Entity() = default;

    // stable identifier for lookup/logging
    virtual Id id() const = 0;

    // runtime category
    virtual EntityKind kind() const = 0;

    // human-readable label
    virtual const std::string& label() const = 0;

    // whether this entity can take actions
    virtual bool canAct() const = 0;
};

} // namespace minotaur