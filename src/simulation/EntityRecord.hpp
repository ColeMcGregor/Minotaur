#pragma once

#include <string>

#include "entities/entity.hpp"
#include "world/Cell.hpp"
#include "simulation/CellPos.hpp"

namespace minotaur::simulation {

struct EntityRecord
{
    using Id = minotaur::Entity::Id;

    Id id = 0;
    EntityKind kind = EntityKind::Other;
    std::string label;

    CellPos pos;
    minotaur::world::Direction facing = minotaur::world::Direction::South;

    bool canAct = false;
    bool blocksMovement = false;
    bool active = true;
};

} // namespace minotaur::simulation