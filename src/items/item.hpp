#pragma once
#include <memory>
#include <string>
#include "rules/ActionContext.hpp"   // for ActionContext

namespace minotaur {

/**
 * @brief Base item type. Items *produce* Actions when used.
 * 
 * @author @Cole_McGregor
 * @date 2025-08-20
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 * @note This is a base class for all items
 * @note Items must be used as an action, and cause effects by being held or used
 */

 // Forward declarations
class Action;

class Item {
public:
    virtual ~Item() = default;

    /// Human-readable name for UI/logs.
    virtual const std::string& name() const = 0;

    /// Create the Action this item performs in the current context.
    /// (Return nullptr to indicate "cannot use now".)
    virtual std::unique_ptr<Action> toAction(const ActionContext& ctx) const = 0;

    /// Whether this item is consumed on use (artifacts can override to false, for passove effects)
    virtual bool consumesOnUse() const { return true; }
};

using ItemPtr = std::unique_ptr<Item>;

} 
