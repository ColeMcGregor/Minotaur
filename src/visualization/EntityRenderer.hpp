#pragma once

#include <vector>

#include "raylib.h"

#include "simulation/EntityRecord.hpp"
#include "visualization/MazeRenderer.hpp"

namespace minotaur::visualization {

/**
 * @brief Renders simulation entities on top of the maze.
 *
 * @author @Cole_McGregor
 * @date 2026-04-27
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */
class EntityRenderer
{
public:
    EntityRenderer();
    ~EntityRenderer();

    EntityRenderer(const EntityRenderer&) = delete;
    EntityRenderer& operator=(const EntityRenderer&) = delete;

    void drawEntities(
        const std::vector<simulation::EntityRecord>& entities,
        int originX,
        int originY
    ) const;

private:
    static constexpr int CELL_SIZE = MazeRenderer::CELL_SIZE;

    // 16px sprites scaled to match maze tile scale.
    static constexpr int ENTITY_BASE_SIZE = MazeRenderer::TILE_BASE_SIZE;
    static constexpr int ENTITY_SCALE = MazeRenderer::TILE_SCALE;
    static constexpr int ENTITY_SIZE = ENTITY_BASE_SIZE * ENTITY_SCALE;

    Texture2D playerTexture_{};       // Rogue
    Texture2D minotaurTexture_{};     // Minotaur
    Texture2D chestTexture_{};        // Chest
    Texture2D enterStairTexture_{};   // Enter stairs
    Texture2D exitStairTexture_{};    // Exit stairs
    Texture2D knightTexture_{};       // Dark knight
    // Texture2D phantomTexture_{};   // Phantom
    // Texture2D skellyTexture_{};    // Skelly

    void drawEntity(
        const simulation::EntityRecord& entity,
        int originX,
        int originY
    ) const;

    const Texture2D* textureFor(
        const simulation::EntityRecord& entity
    ) const;
};

} // namespace minotaur::visualization