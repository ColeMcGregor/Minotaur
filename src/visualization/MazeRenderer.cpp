#include "visualization/MazeRenderer.hpp"

#include "raylib.h"

namespace minotaur::visualization {

using namespace minotaur::world;

namespace
{
    void drawTextureScaled(const Texture2D& texture, int x, int y, int width, int height)
    {
        Rectangle src{
            0.0f,
            0.0f,
            static_cast<float>(texture.width),
            static_cast<float>(texture.height)
        };

        Rectangle dst{
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(width),
            static_cast<float>(height)
        };

        DrawTexturePro(texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    }
}

MazeRenderer::MazeRenderer()
{
    floorTexture_ = LoadTexture("src/assets/Floor_Tile.png");
    extBGTexture_ = LoadTexture("src/assets/Ext_Background.png");
    wallTopTexture_ = LoadTexture("src/assets/Wall_Top.png");
    wallBottomTexture_ = LoadTexture("src/assets/Wall_Bottom.png");
    wallLeftTexture_ = LoadTexture("src/assets/Wall_Left.png");
    wallRightTexture_ = LoadTexture("src/assets/Wall_Right.png");

    SetTextureFilter(floorTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(extBGTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(wallTopTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(wallBottomTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(wallLeftTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(wallRightTexture_, TEXTURE_FILTER_POINT);
}

MazeRenderer::~MazeRenderer()
{
    UnloadTexture(floorTexture_);
    UnloadTexture(extBGTexture_);
    UnloadTexture(wallTopTexture_);
    UnloadTexture(wallBottomTexture_);
    UnloadTexture(wallLeftTexture_);
    UnloadTexture(wallRightTexture_);
}

void MazeRenderer::drawCell(const Cell& c, int originX, int originY) const
{
    const int px = originX + c.x * CELL_SIZE;
    const int py = originY + c.y * CELL_SIZE;

    if (c.active)
    {
        drawTextureScaled(floorTexture_, px, py, CELL_SIZE, CELL_SIZE);
    }
    else
    {
        drawTextureScaled(extBGTexture_, px, py, CELL_SIZE, CELL_SIZE);
    }
}

void MazeRenderer::drawWalls(const Cell& c, int originX, int originY) const
{
    if (!c.active) return;

    const int left = originX + c.x * CELL_SIZE;
    const int top = originY + c.y * CELL_SIZE;
    const int right = left + CELL_SIZE;
    const int bottom = top + CELL_SIZE;

    if (c.north)
    {
        drawTextureScaled(wallTopTexture_, left, top, CELL_SIZE, WALL_THICKNESS);
    }

    if (c.south)
    {
        drawTextureScaled(wallBottomTexture_, left, bottom - WALL_THICKNESS, CELL_SIZE, WALL_THICKNESS);
    }

    if (c.west)
    {
        drawTextureScaled(wallLeftTexture_, left, top, WALL_THICKNESS, CELL_SIZE);
    }

    if (c.east)
    {
        drawTextureScaled(wallRightTexture_, right - WALL_THICKNESS, top, WALL_THICKNESS, CELL_SIZE);
    }
}

void MazeRenderer::drawGridLines(int w, int h, int originX, int originY) const
{
    for (int x = 0; x <= w; ++x)
    {
        const int px = originX + x * CELL_SIZE;
        DrawLine(px, originY, px, originY + h * CELL_SIZE, Color{80, 80, 80, 80});
    }

    for (int y = 0; y <= h; ++y)
    {
        const int py = originY + y * CELL_SIZE;
        DrawLine(originX, py, originX + w * CELL_SIZE, py, Color{80, 80, 80, 80});
    }
}

void MazeRenderer::drawMaze(const MazeState& maze, int originX, int originY) const
{
    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            drawCell(maze.cellAt(x, y), originX, originY);
        }
    }

    drawGridLines(maze.width(), maze.height(), originX, originY);

    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            drawWalls(maze.cellAt(x, y), originX, originY);
        }
    }
}

} // namespace minotaur::visualization