#include "visualization/ShaderRenderer.hpp"

#include "raylib.h"
#include "visualization/MazeRenderer.hpp"
#include <cmath>

namespace minotaur::visualization {

using namespace minotaur::world;
using namespace minotaur::simulation;

void ShaderRenderer::draw(
    const MazeState& maze,
    const CellPos& playerPos,
    int originX,
    int originY,
    const LanternConfig& config
) const
{
    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            const Cell& cell = maze.cellAt(x, y);

            if (!cell.active)
            {
                continue;
            }

            const int dx = x - playerPos.x;
            const int dy = y - playerPos.y;
            const float dist = std::sqrt(static_cast<float>(dx * dx + dy * dy));

            if (dist <= config.clearRadius)
            {
                continue;
            }

            const int px = originX + x * MazeRenderer::CELL_SIZE;
            const int py = originY + y * MazeRenderer::CELL_SIZE;

            if (dist <= config.dimRadius)
            {
                DrawRectangle(px, py,
                    MazeRenderer::CELL_SIZE,
                    MazeRenderer::CELL_SIZE,
                    Color{0, 0, 0, config.dimAlpha});
            }
            else
            {
                DrawRectangle(px, py,
                    MazeRenderer::CELL_SIZE,
                    MazeRenderer::CELL_SIZE,
                    Color{0, 0, 0, config.darkAlpha});
            }
        }
    }
}

}