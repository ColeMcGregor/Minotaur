#include <exception>
#include <string>
#include <vector>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "third_party/raygui/src/raygui.h"

#include "visualization/MazePanel.hpp"
#include "visualization/MazeRenderer.hpp"

#include "world/MazeGenerator.hpp"
#include "world/MazeStep.hpp"
#include "world/MazeStepPrinter.hpp"

using namespace minotaur::world;
using namespace minotaur::visualization;

namespace
{
    constexpr int SCREEN_WIDTH = 1500;
    constexpr int SCREEN_HEIGHT = 950;

    constexpr int MAZE_ORIGIN_X = MazePanel::PANEL_X + MazePanel::PANEL_WIDTH + 24;
    constexpr int MAZE_ORIGIN_Y = 24;

    struct HarnessState
    {
        MazeBuildConfig draftConfig{};
        MazeBuildConfig activeConfig{};
        MazeState activeMaze{1, 1};
        std::vector<MazeStep> steps{};
        bool stepMode = false;

        std::string statusText = "Ready.";
        bool hasGeneratedMaze = false;
    };

    bool validateDraftConfig(const MazeBuildConfig& config, std::string& outError)
    {
        if (config.backingWidth <= 0 || config.backingHeight <= 0)
        {
            outError = "Backing grid dimensions must be greater than 0.";
            return false;
        }

        if (config.mazeWidth <= 0 || config.mazeHeight <= 0)
        {
            outError = "Maze dimensions must be greater than 0.";
            return false;
        }

        if (config.mazeWidth > config.backingWidth || config.mazeHeight > config.backingHeight)
        {
            outError = "Maze dimensions cannot exceed backing grid dimensions.";
            return false;
        }

        if (((config.backingWidth - config.mazeWidth) % 2) != 0)
        {
            outError = "Backing width minus maze width must be even.";
            return false;
        }

        if (((config.backingHeight - config.mazeHeight) % 2) != 0)
        {
            outError = "Backing height minus maze height must be even.";
            return false;
        }

        outError.clear();
        return true;
    }

    void tryRegenerate(HarnessState& state)
    {
        std::string validationError;
        if (!validateDraftConfig(state.draftConfig, validationError))
        {
            state.statusText = validationError;
            return;
        }

        try
        {
            state.activeMaze = MazeGenerator::generate(state.draftConfig, state.steps);
            state.activeConfig = state.draftConfig;
            state.hasGeneratedMaze = true;
            state.statusText = state.stepMode
            ? "Generation successful. Step mode enabled."
            : "Generation successful. Instant mode enabled.";

            if (!state.stepMode)
                {
                    for (const MazeStep& step : state.steps)
                    {
                        MazeStepPrinter::printStep(step);
                    }
                }
        }
        catch (const std::exception& ex)
        {
            state.statusText = std::string("Generation failed: ") + ex.what();
        }
    }
}

int main()
{
    HarnessState state;
    MazePanel panel;

    state.draftConfig.backingWidth = 24;
    state.draftConfig.backingHeight = 19;
    state.draftConfig.mazeWidth = 22;
    state.draftConfig.mazeHeight = 17;
    state.draftConfig.shape = MazeShape::Rectangle;
    state.draftConfig.algorithm = MazeAlgorithm::None;

    panel.syncFromConfig(state.draftConfig);
    tryRegenerate(state);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Harness");
    SetTargetFPS(60);

    MazeRenderer renderer;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{20, 20, 20, 255});

        if (state.hasGeneratedMaze)
        {
            renderer.drawMaze(state.activeMaze, MAZE_ORIGIN_X, MAZE_ORIGIN_Y);
        }

        const MazePanel::Result panelResult =
        panel.draw(state.draftConfig, state.activeConfig, state.statusText, state.stepMode);

        if (panelResult.generatePressed)
        {
            tryRegenerate(state);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}