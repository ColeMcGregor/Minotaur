#pragma once

#include <vector>

#include "world/MazeState.hpp"
#include "simulation/EntityRecord.hpp"

namespace minotaur::simulation {

/**
 * @brief Holds the full runtime simulation state.
 */
class SimState
{
public:

    SimState() : maze(1, 1) {} // minimal valid placeholder
    explicit SimState(world::MazeState mazeState)  //constructor
        : maze(std::move(mazeState)) {}

    world::MazeState maze;
    std::vector<EntityRecord> entities;

    // lookup by id
    EntityRecord* findById(EntityRecord::Id id)
    {
        for (auto& e : entities)
        {
            if (e.id == id) return &e;
        }
        return nullptr;
    }

    const EntityRecord* findById(EntityRecord::Id id) const
    {
        for (const auto& e : entities)
        {
            if (e.id == id) return &e;
        }
        return nullptr;
    }

    // get all entities at a position
    std::vector<EntityRecord*> entitiesAt(const CellPos& pos)
    {
        std::vector<EntityRecord*> result;
        for (auto& e : entities)
        {
            if (e.active && e.pos == pos)
            {
                result.push_back(&e);
            }
        }
        return result;
    }

    // any entity present
    bool hasEntityAt(const CellPos& pos) const
    {
        for (const auto& e : entities)
        {
            if (e.active && e.pos == pos)
            {
                return true;
            }
        }
        return false;
    }

    // movement blocking check
    bool blocksMovementAt(const CellPos& pos) const
    {
        for (const auto& e : entities)
        {
            if (e.active && e.blocksMovement && e.pos == pos)
            {
                return true;
            }
        }
        return false;
    }
};

} // namespace minotaur::simulation