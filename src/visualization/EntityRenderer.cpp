#include "visualization/EntityRenderer.hpp"

#include "raylib.h"
#include "world/Cell.hpp"

namespace minotaur::visualization {

using namespace minotaur::simulation;

namespace
{
    float rotationForFacing(world::Direction direction)
    {
        // Assumes the source sprite's default/open-facing direction is South.
        switch (direction)
        {
            case world::Direction::South: return 0.0f;
            case world::Direction::West:  return 90.0f;
            case world::Direction::North: return 180.0f;
            case world::Direction::East:  return 270.0f;
        }

        return 0.0f;
    }

    bool shouldRotateEntity(const EntityRecord& entity)
    {
        return entity.kind == EntityKind::EnterStair
            || entity.kind == EntityKind::ExitStair;
    }

    void drawTextureScaled(
        const Texture2D& texture,
        int x,
        int y,
        int width,
        int height,
        float rotationDegrees = 0.0f
    )
    {
        Rectangle src{
            0.0f,
            0.0f,
            static_cast<float>(texture.width),
            static_cast<float>(texture.height)
        };

        Rectangle dst{
            static_cast<float>(x + width / 2),
            static_cast<float>(y + height / 2),
            static_cast<float>(width),
            static_cast<float>(height)
        };

        Vector2 origin{
            static_cast<float>(width) / 2.0f,
            static_cast<float>(height) / 2.0f
        };

        DrawTexturePro(texture, src, dst, origin, rotationDegrees, WHITE);
    }
}

EntityRenderer::EntityRenderer()
{
    playerTexture_ = LoadTexture("src/assets/Rogue.png");
    minotaurTexture_ = LoadTexture("src/assets/Minotaur.png");
    chestTexture_ = LoadTexture("src/assets/Chest.png");
    enterStairTexture_ = LoadTexture("src/assets/Stairs_Entrance.png");
    exitStairTexture_ = LoadTexture("src/assets/Stairs_Exit.png");
    knightTexture_ = LoadTexture("src/assets/Knight.png");

    SetTextureFilter(playerTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(minotaurTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(chestTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(enterStairTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(exitStairTexture_, TEXTURE_FILTER_POINT);
    SetTextureFilter(knightTexture_, TEXTURE_FILTER_POINT);
}

EntityRenderer::~EntityRenderer()
{
    UnloadTexture(playerTexture_);
    UnloadTexture(minotaurTexture_);
    UnloadTexture(chestTexture_);
    UnloadTexture(enterStairTexture_);
    UnloadTexture(exitStairTexture_);
    UnloadTexture(knightTexture_);
}

void EntityRenderer::drawEntities(
    const std::vector<EntityRecord>& entities,
    int originX,
    int originY
) const
{
    for (const EntityRecord& entity : entities)
    {
        if (!entity.active)
        {
            continue;
        }

        drawEntity(entity, originX, originY);
    }
}

void EntityRenderer::drawEntity(
        const EntityRecord& entity,
        int originX,
        int originY
    ) const
    {
        const Texture2D* texture = textureFor(entity);

        if (texture == nullptr)
        {
            return;
        }

        const int baseX = originX + entity.pos.x * CELL_SIZE;
        const int baseY = originY + entity.pos.y * CELL_SIZE;

        const int drawWidth = texture->width * MazeRenderer::TILE_SCALE;
        const int drawHeight = texture->height * MazeRenderer::TILE_SCALE;

        const int px = baseX + (CELL_SIZE - drawWidth) / 2;
        const int py = baseY + (CELL_SIZE - drawHeight) / 2;

        const float rotation = shouldRotateEntity(entity)
            ? rotationForFacing(entity.facing)
            : 0.0f;

        drawTextureScaled(*texture, px, py, drawWidth, drawHeight, rotation);
    }

const Texture2D* EntityRenderer::textureFor(const EntityRecord& entity) const
{
    switch (entity.kind)
    {
        case EntityKind::Player:
            return &playerTexture_;

        case EntityKind::Minotaur:
            return &minotaurTexture_;

        case EntityKind::Chest:
            return &chestTexture_;

        case EntityKind::EnterStair:
            return &enterStairTexture_;

        case EntityKind::ExitStair:
            return &exitStairTexture_;

        case EntityKind::Enemy:
            return &knightTexture_;

        default:
            return nullptr;
    }
}

} // namespace minotaur::visualization