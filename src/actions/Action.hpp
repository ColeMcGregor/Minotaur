#pragma once
#include <memory>
#include "rules/ActionContext.hpp"
#include <string>
#include "Result.hpp"
#include "entities/Entity.hpp"

namespace minotaur {

/**
 * Base class for all Actions that an Actor can perform during a turn.
 * Examples: Move, Teleport, PhaseMove, UseItem, OpenChest, etc
 */
class Action {
public:
    virtual ~Action() = default;

    /// Human-readable name for UI/logs.
    virtual std::string name() const = 0;

    /// Cost in Action Points (AP) required to perform this action.
    virtual int costAP() const = 0;

    /// Validate whether the action can be performed given current state.
    /// Does not mutate state.
    virtual Result validate(const Entity& entity, const ActionContext& ctx) const = 0;

    /// Apply the effects of the action (mutates state).
    /// Assumes validate() returned success.
    virtual Result apply(Entity& entity, ActionContext& ctx) = 0;
};

/// Convenience alias for owning pointer type.
using ActionPtr = std::unique_ptr<Action>;

} 
