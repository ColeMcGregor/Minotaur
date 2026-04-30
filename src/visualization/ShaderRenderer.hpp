#pragma once

#include "world/MazeState.hpp"
#include "simulation/CellPos.hpp"

namespace minotaur::visualization {

struct LanternConfig
{
    int clearRadius = 2;
    int dimRadius = 3;
    unsigned char dimAlpha = 120;
    unsigned char darkAlpha = 220;
};

class ShaderRenderer
{
public:
    void draw(
        const minotaur::world::MazeState& maze,
        const minotaur::simulation::CellPos& playerPos,
        int originX,
        int originY,
        const LanternConfig& config
    ) const;
};

}