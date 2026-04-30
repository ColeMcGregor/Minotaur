#include "simulation/SimGenerator.hpp"

#include <stdexcept>
#include <vector>

#include "world/MazeGenerator.hpp"

namespace minotaur::simulation {

using namespace minotaur::world;

namespace
{
    bool isWallClosed(const Cell& cell, Direction direction)
    {
        switch (direction)
        {
            case Direction::North: return cell.north;
            case Direction::East:  return cell.east;
            case Direction::South: return cell.south;
            case Direction::West:  return cell.west;
        }

        return true;
    }

    bool isDeadEndCell(const Cell& cell)
    {
        if (!cell.active)
        {
            return false;
        }

        int closedCount = 0;

        if (cell.north) ++closedCount;
        if (cell.east)  ++closedCount;
        if (cell.south) ++closedCount;
        if (cell.west)  ++closedCount;

        return closedCount == 3;
    }

    Direction openDirectionForDeadEnd(const Cell& cell)
    {
        const Direction directions[] = {
            Direction::North,
            Direction::East,
            Direction::South,
            Direction::West
        };

        for (Direction direction : directions)
        {
            if (!isWallClosed(cell, direction))
            {
                return direction;
            }
        }

        return Direction::South;
    }
}

SimState SimGenerator::generate(
    const SimBuildConfig& config,
    RNG& rng,
    std::vector<MazeStep>& steps
)
{
    SimState sim;

    sim.maze = MazeGenerator::generate(config.maze, steps);

    const CellPos enterPos = placeEnterStair(sim, rng);
    placeExitStair(sim, enterPos, config, rng);
    placePlayerAtEnterStair(sim, enterPos);

    return sim;
}

CellPos SimGenerator::placeEnterStair(SimState& sim, RNG& rng)
{
    std::vector<CellPos> candidates;

    for (int y = 0; y < sim.maze.height(); ++y)
    {
        for (int x = 0; x < sim.maze.width(); ++x)
        {
            const Cell& cell = sim.maze.cellAt(x, y);

            if (isDeadEndCell(cell))
            {
                candidates.push_back(CellPos{x, y});
            }
        }
    }

    if (candidates.empty())
    {
        throw std::runtime_error("No dead-end cells available for EnterStair placement.");
    }

    const int idx = rng.uniformInt(0, static_cast<int>(candidates.size()) - 1);
    const CellPos pos = candidates[idx];

    const Cell& cell = sim.maze.cellAt(pos.x, pos.y);
    const Direction openDirection = openDirectionForDeadEnd(cell);

    EntityRecord& stair = addEntity(
        sim,
        EntityKind::EnterStair,
        "Enter Stair",
        pos,
        false,
        false
    );

    stair.facing = openDirection;

    return pos;
}

void SimGenerator::placePlayerAtEnterStair(SimState& sim, const CellPos& enterPos)
{
    EntityRecord& player = addEntity(
        sim,
        EntityKind::Player,
        "Player",
        enterPos,
        true,   // canAct
        true    // blocksMovement
    );

    player.facing = minotaur::world::Direction::South;
}

CellPos SimGenerator::placeExitStair(
    SimState& sim,
    const CellPos& enterPos,
    const SimBuildConfig& config,
    RNG& rng
)
{
    std::vector<CellPos> candidates;

    for (int y = 0; y < sim.maze.height(); ++y)
    {
        for (int x = 0; x < sim.maze.width(); ++x)
        {
            const Cell& cell = sim.maze.cellAt(x, y);

            if (!isDeadEndCell(cell))
            {
                continue;
            }

            CellPos pos{x, y};

            if (sim.hasEntityAt(pos))
            {
                continue;
            }

            const int dx = std::abs(x - enterPos.x);
            const int dy = std::abs(y - enterPos.y);

            if (dx + dy < config.minExitManhattanDistance)
            {
                continue;
            }

            if (config.forbidExitSameAxis && (dx == 0 || dy == 0))
            {
                continue;
            }

            candidates.push_back(pos);
        }
    }

    if (candidates.empty())
    {
        throw std::runtime_error("No valid dead-end cells available for ExitStair placement.");
    }

    const int idx = rng.uniformInt(0, static_cast<int>(candidates.size()) - 1);
    const CellPos pos = candidates[idx];

    const Cell& cell = sim.maze.cellAt(pos.x, pos.y);
    const Direction openDirection = openDirectionForDeadEnd(cell);

    EntityRecord& stair = addEntity(
        sim,
        EntityKind::ExitStair,
        "Exit Stair",
        pos,
        false,
        false
    );

    stair.facing = openDirection;

    return pos;
}

CellPos SimGenerator::randomActiveCell(const MazeState& maze, RNG& rng)
{
    std::vector<CellPos> candidates;

    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            const Cell& c = maze.cellAt(x, y);

            if (c.active)
            {
                candidates.push_back(CellPos{x, y});
            }
        }
    }

    if (candidates.empty())
    {
        throw std::runtime_error("No active cells available for placement.");
    }

    const int idx = rng.uniformInt(0, static_cast<int>(candidates.size()) - 1);
    return candidates[idx];
}

EntityRecord& SimGenerator::addEntity(
    SimState& sim,
    EntityKind kind,
    const std::string& label,
    const CellPos& pos,
    bool canAct,
    bool blocksMovement
)
{
    EntityRecord e;

    e.id = nextEntityId(sim);
    e.kind = kind;
    e.label = label;
    e.pos = pos;
    e.canAct = canAct;
    e.blocksMovement = blocksMovement;
    e.active = true;

    sim.entities.push_back(e);
    return sim.entities.back();
}

EntityRecord::Id SimGenerator::nextEntityId(SimState& sim)
{
    return static_cast<EntityRecord::Id>(sim.entities.size() + 1);
}

} // namespace minotaur::simulation