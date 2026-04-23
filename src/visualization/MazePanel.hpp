#pragma once

#include <string>

#include "world/MazeBuildConfig.hpp"

namespace minotaur::visualization {

/**
 * @brief Left-side harness panel for editing maze generation inputs
 * @author @Cole_McGregor
 * @date 2026-04-21
 * @version 1.1.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */

class MazePanel
{
public:
    static constexpr int PANEL_X = 16;
    static constexpr int PANEL_Y = 16;
    static constexpr int PANEL_WIDTH = 280;
    static constexpr int PANEL_HEIGHT = 918;

    struct Result
    {
        bool generatePressed = false;
    };

    MazePanel() = default;

    void syncFromConfig(const minotaur::world::MazeBuildConfig& config);
    void syncToConfig(minotaur::world::MazeBuildConfig& config) const;

    Result draw(
        minotaur::world::MazeBuildConfig& draftConfig,
        const minotaur::world::MazeBuildConfig& activeConfig,
        const std::string& statusText,
        bool& stepMode
    );

private:
    bool editBackingWidth_ = false;
    bool editBackingHeight_ = false;
    bool editMazeWidth_ = false;
    bool editMazeHeight_ = false;

    int shapeDropdownActive_ = 0;
    bool shapeDropdownEdit_ = false;

    int algorithmDropdownActive_ = 0;
    bool algorithmDropdownEdit_ = false;
};

} // namespace minotaur::visualization