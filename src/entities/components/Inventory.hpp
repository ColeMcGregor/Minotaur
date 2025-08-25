#pragma once
#include <memory>
#include <string>

namespace minotaur
{
class Item;

namespace entities::components
{

/**
 * Inventory is a component that manages the inventory of an entity.
 * It is responsible for adding, taking, and consuming items, and for finding items by name.
 * It is used for any entity that can carry items, such as a player, a monster, or a chest.
 * 
 * @author: @Cole_McGregor
 * @date: 2025-08-24
 * @version: 1.0.0
 * @copyright: Copyright (c) 2025 Cole McGregor
 * @note: this is a component, not a system.
 */

class Inventory
{
public:
    virtual ~Inventory() = default;

    virtual int size() const = 0;
    virtual int capacity() const = 0;                 // -1 => unlimited, should differ by entity type

    virtual bool add(std::unique_ptr<Item> it) = 0;   // put into inventory; false if full/denied
    virtual std::unique_ptr<Item> takeByIndex(int index) = 0; // take out of inventory; nullptr if bad index
    virtual bool consumeByIndex(int index) = 0;       // erase item from inventory (used/Destroyed); false if bad index

    virtual const Item* peek(int index) const = 0;    // read-only view for UI
    virtual int findFirstByName(const std::string& name) const = 0; // find first item by name in inventory

    virtual void clear() = 0; // remove all items from inventory
};

} 
} 
