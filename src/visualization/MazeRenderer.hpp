#pragma once

#include "raylib.h"
#include "world/MazeState.hpp"

namespace minotaur::visualization {

/**
 * @brief Draws a MazeState using raylib
 * @author @Cole_McGregor
 * @date 2026-04-21
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

class MazeRenderer
{
public:
    static constexpr int TILE_BASE_SIZE = 16;
    static constexpr int WALL_BASE_THICKNESS = 2;
    static constexpr int TILE_SCALE = 3; 

    static constexpr int CELL_SIZE = TILE_BASE_SIZE * TILE_SCALE;
    static constexpr int WALL_THICKNESS = WALL_BASE_THICKNESS * TILE_SCALE;

    MazeRenderer();
    ~MazeRenderer();

    MazeRenderer(const MazeRenderer&) = delete;
    MazeRenderer& operator=(const MazeRenderer&) = delete;

    void drawMaze(const minotaur::world::MazeState& maze, int originX, int originY) const;

private:
    void drawCell(const minotaur::world::Cell& c, int originX, int originY) const;
    void drawWalls(const minotaur::world::Cell& c, int originX, int originY) const;
    void drawGridLines(int w, int h, int originX, int originY) const;

    Texture2D floorTexture_{};
    Texture2D extBGTexture_{};
    Texture2D wallTopTexture_{};
    Texture2D wallBottomTexture_{};
    Texture2D wallLeftTexture_{};
    Texture2D wallRightTexture_{};
};

} // namespace minotaur::visualization