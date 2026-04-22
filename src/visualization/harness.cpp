#include <exception>
#include <string>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "third_party/raygui/src/raygui.h"

#include "world/MazeGenerator.hpp"

using namespace minotaur::world;

// ----- layout -----
static constexpr int SCREEN_WIDTH = 1500;
static constexpr int SCREEN_HEIGHT = 950;

static constexpr int PANEL_X = 16;
static constexpr int PANEL_Y = 16;
static constexpr int PANEL_WIDTH = 280;
static constexpr int PANEL_HEIGHT = SCREEN_HEIGHT - 32;

static constexpr int MAZE_ORIGIN_X = PANEL_X + PANEL_WIDTH + 24;
static constexpr int MAZE_ORIGIN_Y = 24;

static constexpr int CELL_SIZE = 28;
static constexpr int WALL_THICKNESS = 2;

// ----- UI state -----

struct HarnessUiState
{
    MazeBuildConfig draftConfig{};
    MazeBuildConfig activeConfig{};

    MazeState activeMaze{1, 1};

    bool editBackingWidth = false;
    bool editBackingHeight = false;
    bool editMazeWidth = false;
    bool editMazeHeight = false;

    int shapeDropdownActive = 0;
    bool shapeDropdownEdit = false;

    std::string statusText = "Ready.";
    bool hasGeneratedMaze = false;
};

// ----- validation helpers -----

bool ValidateDraftConfig(const MazeBuildConfig& config, std::string& outError)
{
    if (config.backingWidth <= 0 || config.backingHeight <= 0)
    {
        outError = "Backing grid dimensions must be greater than 0.";
        return false;
    }

    if (config.mazeWidth <= 0 || config.mazeHeight <= 0)
    {
        outError = "Maze dimensions must \n be greater than 0.";
        return false;
    }

    if (config.mazeWidth > config.backingWidth || config.mazeHeight > config.backingHeight)
    {
        outError = "Maze dimensions cannot \n exceed backing grid \n dimensions.";
        return false;
    }

    if (((config.backingWidth - config.mazeWidth) % 2) != 0)
    {
        outError = "Backing width minus \n maze width must be even.";
        return false;
    }

    if (((config.backingHeight - config.mazeHeight) % 2) != 0)
    {
        outError = "Backing height minus \n maze height must be even.";
        return false;
    }

    outError.clear();
    return true;
}

void TryRegenerate(HarnessUiState& ui)
{
    std::string validationError;
    if (!ValidateDraftConfig(ui.draftConfig, validationError))
    {
        ui.statusText = validationError;
        return;
    }

    try
    {
        ui.activeMaze = MazeGenerator::generate(ui.draftConfig);
        ui.activeConfig = ui.draftConfig;
        ui.hasGeneratedMaze = true;
        ui.statusText = "Generation successful.";
    }
    catch (const std::exception& ex)
    {
        ui.statusText = std::string("Generation failed: ") + ex.what();
    }
}

// ----- draw helpers -----

void DrawCell(const Cell& c, int originX, int originY)
{
    const int px = originX + c.x * CELL_SIZE;
    const int py = originY + c.y * CELL_SIZE;

    if (c.active)
    {
        DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, Color{185, 185, 185, 255});
    }
    else
    {
        DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, Color{40, 40, 40, 255});
    }
}

void DrawWalls(const Cell& c, int originX, int originY)
{
    if (!c.active) return;

    const int left = originX + c.x * CELL_SIZE;
    const int top = originY + c.y * CELL_SIZE;
    const int right = left + CELL_SIZE;
    const int bottom = top + CELL_SIZE;

    if (c.north)
        DrawRectangle(left, top, CELL_SIZE, WALL_THICKNESS, RAYWHITE);

    if (c.south)
        DrawRectangle(left, bottom - WALL_THICKNESS, CELL_SIZE, WALL_THICKNESS, RAYWHITE);

    if (c.west)
        DrawRectangle(left, top, WALL_THICKNESS, CELL_SIZE, RAYWHITE);

    if (c.east)
        DrawRectangle(right - WALL_THICKNESS, top, WALL_THICKNESS, CELL_SIZE, RAYWHITE);
}

void DrawGridLines(int w, int h, int originX, int originY)
{
    for (int x = 0; x <= w; ++x)
    {
        const int px = originX + x * CELL_SIZE;
        DrawLine(px, originY, px, originY + h * CELL_SIZE, Color{80, 80, 80, 255});
    }

    for (int y = 0; y <= h; ++y)
    {
        const int py = originY + y * CELL_SIZE;
        DrawLine(originX, py, originX + w * CELL_SIZE, py, Color{80, 80, 80, 255});
    }
}

void DrawMaze(const MazeState& maze, int originX, int originY)
{
    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            const Cell& c = maze.cellAt(x, y);
            DrawCell(c, originX, originY);
        }
    }

    DrawGridLines(maze.width(), maze.height(), originX, originY);

    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            const Cell& c = maze.cellAt(x, y);
            DrawWalls(c, originX, originY);
        }
    }
}

void DrawUiPanel(HarnessUiState& ui)
{
    Rectangle panel = {
        static_cast<float>(PANEL_X),
        static_cast<float>(PANEL_Y),
        static_cast<float>(PANEL_WIDTH),
        static_cast<float>(PANEL_HEIGHT)
    };

    GuiPanel(panel, "Maze Harness");

    int x = PANEL_X + 16;
    int y = PANEL_Y + 36;
    const int labelWidth = 110;
    const int valueWidth = 120;
    const int rowHeight = 32;
    const int gap = 10;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Backing Grid");
    y += 24;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Width");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &ui.draftConfig.backingWidth,
        1,
        200,
        ui.editBackingWidth
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, ui.editBackingWidth ? "OK" : "..."))
    {
        ui.editBackingWidth = !ui.editBackingWidth;
        if (ui.editBackingWidth)
        {
            ui.editBackingHeight = false;
            ui.editMazeWidth = false;
            ui.editMazeHeight = false;
        }
    }
    y += rowHeight + gap;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Height");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &ui.draftConfig.backingHeight,
        1,
        200,
        ui.editBackingHeight
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, ui.editBackingHeight ? "OK" : "..."))
    {
        ui.editBackingHeight = !ui.editBackingHeight;
        if (ui.editBackingHeight)
        {
            ui.editBackingWidth = false;
            ui.editMazeWidth = false;
            ui.editMazeHeight = false;
        }
    }
    y += rowHeight + 20;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Maze Footprint");
    y += 24;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Width");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &ui.draftConfig.mazeWidth,
        1,
        200,
        ui.editMazeWidth
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, ui.editMazeWidth ? "OK" : "..."))
    {
        ui.editMazeWidth = !ui.editMazeWidth;
        if (ui.editMazeWidth)
        {
            ui.editBackingWidth = false;
            ui.editBackingHeight = false;
            ui.editMazeHeight = false;
        }
    }
    y += rowHeight + gap;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Height");
    GuiValueBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth), static_cast<float>(rowHeight)},
        nullptr,
        &ui.draftConfig.mazeHeight,
        1,
        200,
        ui.editMazeHeight
    );
    if (GuiButton(Rectangle{static_cast<float>(x + labelWidth + valueWidth + 8), static_cast<float>(y), 28, static_cast<float>(rowHeight)}, ui.editMazeHeight ? "OK" : "..."))
    {
        ui.editMazeHeight = !ui.editMazeHeight;
        if (ui.editMazeHeight)
        {
            ui.editBackingWidth = false;
            ui.editBackingHeight = false;
            ui.editMazeWidth = false;
        }
    }
    y += rowHeight + 20;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y + 6), static_cast<float>(labelWidth), 20}, "Shape");
    if (GuiDropdownBox(
        Rectangle{static_cast<float>(x + labelWidth), static_cast<float>(y), static_cast<float>(valueWidth + 36), static_cast<float>(rowHeight)},
        "Rectangle;Ellipse",
        &ui.shapeDropdownActive,
        ui.shapeDropdownEdit
    ))
    {
        ui.shapeDropdownEdit = !ui.shapeDropdownEdit;
    }

    ui.draftConfig.shape = (ui.shapeDropdownActive == 0) ? MazeShape::Rectangle : MazeShape::Ellipse;

    y += rowHeight + 24;

    if (GuiButton(Rectangle{static_cast<float>(x), static_cast<float>(y), 240.0f, 36.0f}, "Generate"))
    {
        ui.editBackingWidth = false;
        ui.editBackingHeight = false;
        ui.editMazeWidth = false;
        ui.editMazeHeight = false;
        ui.shapeDropdownEdit = false;
        TryRegenerate(ui);
    }

    y += 52;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Rules");
    y += 22;
    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "- Maze dims must fit backing grid");
    y += 18;
    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "- Backing minus maze dims must be even");
    y += 26;

    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Status");
    y += 22;
    DrawText(ui.statusText.c_str(), x, y, 18, BLACK);

    y += 44;
    GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 240, 20}, "Last Generated");
    y += 22;

    const std::string summary =
        "Backing: " + std::to_string(ui.activeConfig.backingWidth) + " x " + std::to_string(ui.activeConfig.backingHeight) +
        "\nMaze: " + std::to_string(ui.activeConfig.mazeWidth) + " x " + std::to_string(ui.activeConfig.mazeHeight) +
        "\nShape: " + std::string((ui.activeConfig.shape == MazeShape::Rectangle) ? "Rectangle" : "Ellipse");

    DrawText(summary.c_str(), x, y, 18, Color{200, 200, 200, 255});
}

int main()
{
    HarnessUiState ui;

    ui.draftConfig.backingWidth = 30;
    ui.draftConfig.backingHeight = 30;
    ui.draftConfig.mazeWidth = 24;
    ui.draftConfig.mazeHeight = 24;
    ui.draftConfig.shape = MazeShape::Rectangle;

    ui.shapeDropdownActive = 0;

    TryRegenerate(ui);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Harness");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{20, 20, 20, 255});

        if (ui.hasGeneratedMaze)
        {
            DrawMaze(ui.activeMaze, MAZE_ORIGIN_X, MAZE_ORIGIN_Y);
        }

        DrawUiPanel(ui);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}