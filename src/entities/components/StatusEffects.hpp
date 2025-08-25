#pragma once
#include <memory>

namespace minotaur
{
namespace effects { class Effect; }

namespace entities::components
{

class Stats;  // forward declaration

/**
 * StatusEffects is a component that manages the status effects of an entity.
 * It is responsible for applying and removing status effects, and for ticking and sweeping expired effects.
 * it is kind of like an inventory for changes on stats, indefinite or otherwise
 * 
 * @author: @Cole_McGregor
 * @date: 2025-08-24
 * @version: 1.0.0
 * @copyright: Copyright (c) 2025 Cole McGregor
 * @note: this is a component, not a system.
 */

class StatusEffects
{
public:
    virtual ~StatusEffects() = default;

    virtual int count() const = 0;

    // Apply-on-add: calls Effect::onApply(stats) if accepted, as effects affect stats
    virtual bool add(std::unique_ptr<effects::Effect> e, Stats& stats) = 0;

    // Remove specific index: calls Effect::onRemove(stats) before erasing
    virtual bool removeByIndex(int index, Stats& stats) = 0;

    // Tick all effects; then sweep expired (calling onRemove)
    virtual void tick(Stats& stats) = 0;

    // Remove all, calling onRemove for each
    virtual void clear(Stats& stats) = 0;

    // Read-only peek for UI/logs
    virtual const effects::Effect* peek(int index) const = 0;
};

} // namespace entities::components
} // namespace minotaur
