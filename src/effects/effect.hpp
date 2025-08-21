#pragma once
#include <string>

namespace minotaur {

/**
 * @brief A base status effect (e.g., +AP, Phase-through-wall).
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a base class for all status effects
 */

// Forward declarations
namespace entities { struct Stats; }

/**
 * Base status effect (e.g., +AP, Phase-through-wall).
 */
class Effect {
public:
    virtual ~Effect() = default;

    /// Human-readable name for logs/UI.
    virtual const std::string& name() const = 0;

    /// Called once when the effect is applied (e.g., add AP bonus).
    virtual void onApply(entities::Stats& stats) = 0;

    /// Called at a regular cadence (e.g., per turn) to update duration, etc.
    virtual void onTick(entities::Stats& /*stats*/) {}

    /// Called once when the effect is removed/expired (e.g., undo AP bonus).
    virtual void onRemove(entities::Stats& /*stats*/) {}

    /// True if the effect should be removed by the owner.
    virtual bool expired() const { return false; }
};

} 
