#pragma once
#include <memory>
#include <string>
#include "rules/ActionContext.hpp"   // for ActionContext

namespace minotaur {

/**
 * @brief Base item type. Items are played as Actions and/or grant passive Effects.
 *
 * Examples:
 *  - TeleportCharm: toAction(ctx) -> TeleportAction (blink to any visible cell).
 *  - HasteAmulet: grantsPassive() -> true; makePassiveEffect() -> +AP effect while equipped.
 *  - FogLantern:   grantsPassive() -> true; makePassiveEffect() -> increases FOV radius.
 *
 * Author:   @Cole_McGregor
 * Date:     2025-08-21
 * Version:  1.0.1
 * Note:     Items must be used via an Action to do active things; some also apply passive Effects.
 */

// Forward declarations (keep headers light)
class Action;
class Effect;

class Item {
public:
    virtual ~Item() = default;

    /// Human-readable name for UI/logs.
    virtual const std::string& name() const = 0;

    /**
     * Create the Action this item performs when actively used (e.g., Teleport).
     * Return nullptr to indicate "cannot use now" (e.g., no valid target in sight).
     * Validation of legality is still done by the Action/RulesEngine.
     */
    virtual std::unique_ptr<Action> toAction(const ActionContext& ctx) const = 0;

    /**
     * Passive effect support (while held/equipped).
     * Override to return true and supply a concrete Effect via makePassiveEffect().
     */
    virtual bool grantsPassive() const { return false; }

    /// Build the passive Effect instance this item provides (nullptr if none).
    virtual std::unique_ptr<Effect> makePassiveEffect() const { return nullptr; }

    /// Whether the item is consumed on active use (artifacts can override to false).
    virtual bool consumesOnUse() const { return true; }
};

using ItemPtr = std::unique_ptr<Item>;

} // namespace minotaur
