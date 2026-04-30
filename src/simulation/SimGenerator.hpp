#pragma once

#include <vector>

#include "entities/entity.hpp"
#include "core/RNG.hpp"

#include "simulation/CellPos.hpp"
#include "simulation/EntityRecord.hpp"
#include "simulation/SimBuildConfig.hpp"
#include "simulation/SimState.hpp"

#include "world/MazeStep.hpp"

namespace minotaur::simulation {

/**
 * @brief Builds a SimState from maze generation plus initial entity placement.
 *
 * Initial milestone:
 * - generate maze
 * - place EnterStair
 *
 * Later:
 * - place ExitStair
 * - place Player
 * - place Minotaur
 * - place chests/enemies/etc.
 */
class SimGenerator
{
public:
    static SimState generate(
        const SimBuildConfig& config,
        RNG& rng,
        std::vector<world::MazeStep>& steps
    );

private:
    static CellPos placeEnterStair(SimState& sim, RNG& rng);
    static void placePlayerAtEnterStair(SimState& sim, const CellPos& enterPos);
    static CellPos placeExitStair(SimState& sim, const CellPos& enterPos, const SimBuildConfig& config, RNG& rng);

    static CellPos randomActiveCell(const world::MazeState& maze, RNG& rng);

    static EntityRecord& addEntity(
        SimState& sim,
        EntityKind kind,
        const std::string& label,
        const CellPos& pos,
        bool canAct,
        bool blocksMovement
    );

    static EntityRecord::Id nextEntityId(SimState& sim);
};

} // namespace minotaur::simulation