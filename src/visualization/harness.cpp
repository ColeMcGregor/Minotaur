#include <exception>
#include <string>
#include <vector>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "third_party/raygui/src/raygui.h"

#include "core/RNG.hpp"

#include "simulation/SimBuildConfig.hpp"
#include "simulation/SimGenerator.hpp"
#include "simulation/SimState.hpp"

#include "visualization/EntityRenderer.hpp"
#include "visualization/MazePanel.hpp"
#include "visualization/MazeRenderer.hpp"

#include "world/MazeBuildConfig.hpp"
#include "world/MazeStep.hpp"
#include "world/MazeStepPrinter.hpp"

using namespace minotaur::world;
using namespace minotaur::visualization;
using namespace minotaur::simulation;

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
        SimState activeSim{};

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

    void tryRegenerate(HarnessState& state, minotaur::RNG& rng)
    {
        std::string validationError;
        if (!validateDraftConfig(state.draftConfig, validationError))
        {
            state.statusText = validationError;
            return;
        }

        try
        {
            state.steps.clear();

            SimBuildConfig simConfig;
            simConfig.maze = state.draftConfig;

            state.activeSim = SimGenerator::generate(simConfig, rng, state.steps);
            state.activeConfig = state.draftConfig;
            state.hasGeneratedMaze = true;

            state.statusText = state.stepMode
                ? "Generation successful. \n Step mode enabled."
                : "Generation successful.\n  Instant mode enabled.";

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
    minotaur::RNG rng;

    state.draftConfig.backingWidth = 24;
    state.draftConfig.backingHeight = 19;
    state.draftConfig.mazeWidth = 12;
    state.draftConfig.mazeHeight = 9;
    state.draftConfig.shape = MazeShape::Rectangle;
    state.draftConfig.algorithm = MazeAlgorithm::None;

    panel.syncFromConfig(state.draftConfig);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Harness");
    SetTargetFPS(60);

    MazeRenderer mazeRenderer;
    EntityRenderer entityRenderer;

    tryRegenerate(state, rng);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{20, 20, 20, 255});

        if (state.hasGeneratedMaze)
        {
            mazeRenderer.drawMaze(state.activeSim.maze, MAZE_ORIGIN_X, MAZE_ORIGIN_Y);
            entityRenderer.drawEntities(state.activeSim.entities, MAZE_ORIGIN_X, MAZE_ORIGIN_Y);
        }

        const MazePanel::Result panelResult =
            panel.draw(state.draftConfig, state.activeConfig, state.statusText, state.stepMode);

        if (panelResult.generatePressed)
        {
            tryRegenerate(state, rng);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}