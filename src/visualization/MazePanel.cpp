#include "visualization/MazePanel.hpp"

#include "raylib.h"
#include "third_party/raygui/src/raygui.h"

#include "world/MazeAlgorithm.hpp"
#include "world/MazeShape.hpp"

namespace minotaur::visualization {

using namespace minotaur::world;

void MazePanel::syncFromConfig(const MazeBuildConfig& config)
{
    shapeDropdownActive_ = (config.shape == MazeShape::Rectangle) ? 0 : 1;

    switch (config.algorithm)
    {
        case MazeAlgorithm::None:       algorithmDropdownActive_ = 0; break;
        case MazeAlgorithm::DepthFirst: algorithmDropdownActive_ = 1; break;
        default:                        algorithmDropdownActive_ = 0; break;
    }
}

void MazePanel::syncToConfig(MazeBuildConfig& config) const
{
    config.shape =
        (shapeDropdownActive_ == 0)
            ? MazeShape::Rectangle
            : MazeShape::Ellipse;

    switch (algorithmDropdownActive_)
    {
        case 0: config.algorithm = MazeAlgorithm::None; break;
        case 1: config.algorithm = MazeAlgorithm::DepthFirst; break;
        default: config.algorithm = MazeAlgorithm::None; break;
    }
}

MazePanel::Result MazePanel::draw(
    MazeBuildConfig& draftConfig,
    const MazeBuildConfig& activeConfig,
    const std::string& statusText
)
{
    Result result{};

    syncFromConfig(draftConfig);

    Rectangle panel = {
        static_cast<float>(PANEL_X),
        static_cast<float>(PANEL_Y),
        static_cast<float>(PANEL_WIDTH),
        static_cast<float>(PANEL_HEIGHT)
    };

    GuiPanel(panel, "Maze Harness");

    const int x = PANEL_X + 16;
    int y = PANEL_Y + 36;
    const int labelWidth = 110;
    const int valueWidth = 120;
    const int rowHeight = 32;
    const int gap = 10;

    const int buttonWidth = PANEL_WIDTH - 32;
    const int buttonHeight = 40;
    const int buttonY = PANEL_Y + PANEL_HEIGHT - 16 - buttonHeight;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Backing Grid");
    y += 24;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Width");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &draftConfig.backingWidth,
        1,
        200,
        editBackingWidth_
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, editBackingWidth_ ? "OK" : "..."))
    {
        editBackingWidth_ = !editBackingWidth_;
        if (editBackingWidth_)
        {
            editBackingHeight_ = false;
            editMazeWidth_ = false;
            editMazeHeight_ = false;
        }
    }
    y += rowHeight + gap;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Height");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &draftConfig.backingHeight,
        1,
        200,
        editBackingHeight_
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, editBackingHeight_ ? "OK" : "..."))
    {
        editBackingHeight_ = !editBackingHeight_;
        if (editBackingHeight_)
        {
            editBackingWidth_ = false;
            editMazeWidth_ = false;
            editMazeHeight_ = false;
        }
    }
    y += rowHeight + 20;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Maze Footprint");
    y += 24;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Width");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &draftConfig.mazeWidth,
        1,
        200,
        editMazeWidth_
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, editMazeWidth_ ? "OK" : "..."))
    {
        editMazeWidth_ = !editMazeWidth_;
        if (editMazeWidth_)
        {
            editBackingWidth_ = false;
            editBackingHeight_ = false;
            editMazeHeight_ = false;
        }
    }
    y += rowHeight + gap;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Height");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &draftConfig.mazeHeight,
        1,
        200,
        editMazeHeight_
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, editMazeHeight_ ? "OK" : "..."))
    {
        editMazeHeight_ = !editMazeHeight_;
        if (editMazeHeight_)
        {
            editBackingWidth_ = false;
            editBackingHeight_ = false;
            editMazeWidth_ = false;
        }
    }
    y += rowHeight + 20;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Shape");
    if (GuiDropdownBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth + 36), static_cast<float>(rowHeight)},
        "Rectangle;Ellipse",
        &shapeDropdownActive_,
        shapeDropdownEdit_
    ))
    {
        shapeDropdownEdit_ = !shapeDropdownEdit_;
        algorithmDropdownEdit_ = false;
    }
    y += rowHeight + 12;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Algorithm");
    if (GuiDropdownBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth + 36), static_cast<float>(rowHeight)},
        "None;DepthFirst",
        &algorithmDropdownActive_,
        algorithmDropdownEdit_
    ))
    {
        algorithmDropdownEdit_ = !algorithmDropdownEdit_;
        shapeDropdownEdit_ = false;
    }

    syncToConfig(draftConfig);

    y += rowHeight + 24;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Rules");
    y += 22;
    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "- Maze dims must fit backing grid");
    y += 18;
    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "- Backing minus maze dims must be even");
    y += 26;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Status");
    y += 22;
    DrawText(statusText.c_str(), x, y, 18, RAYWHITE);

    y += 44;
    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Last Generated");
    y += 22;

    const char* shapeText =
        (activeConfig.shape == MazeShape::Rectangle) ? "Rectangle" : "Ellipse";

    const char* algorithmText =
        (activeConfig.algorithm == MazeAlgorithm::DepthFirst) ? "DepthFirst" : "None";

    const std::string summary =
        "Backing: " + std::to_string(activeConfig.backingWidth) + " x " + std::to_string(activeConfig.backingHeight) +
        "\nMaze: " + std::to_string(activeConfig.mazeWidth) + " x " + std::to_string(activeConfig.mazeHeight) +
        "\nShape: " + std::string(shapeText) +
        "\nAlgorithm: " + std::string(algorithmText);

    DrawText(summary.c_str(), x, y, 18, Color{200, 200, 200, 255});

    if (GuiButton(
        Rectangle{
            static_cast<float>(PANEL_X + 16),
            static_cast<float>(buttonY),
            static_cast<float>(buttonWidth),
            static_cast<float>(buttonHeight)
        },
        "Generate"
    ))
    {
        editBackingWidth_ = false;
        editBackingHeight_ = false;
        editMazeWidth_ = false;
        editMazeHeight_ = false;
        shapeDropdownEdit_ = false;
        algorithmDropdownEdit_ = false;
        result.generatePressed = true;
    }

    return result;
}

} // namespace minotaur::visualization