#include "world/MazeGenerator.hpp"

#include <stdexcept>

#include "world/MazeCarver.hpp"

namespace minotaur::world {

MazeState MazeGenerator::generate(const MazeBuildConfig& config, std::vector<MazeStep>& steps)
{
    validateConfig(config);

    MazeState maze(config.backingWidth, config.backingHeight);

    formShape(maze, config);
    maze.finalizeNeighbors();
    initializeClosedWalls(maze);
    applyWallAlgorithm(maze, config, steps);

    return maze;
}

void MazeGenerator::validateConfig(const MazeBuildConfig& config)
{
    if (config.backingWidth <= 0 || config.backingHeight <= 0)
    {
        throw std::invalid_argument("Backing grid dimensions must be greater than zero.");
    }

    if (config.mazeWidth <= 0 || config.mazeHeight <= 0)
    {
        throw std::invalid_argument("Maze dimensions must be greater than zero.");
    }

    if (config.mazeWidth > config.backingWidth || config.mazeHeight > config.backingHeight)
    {
        throw std::invalid_argument("Maze dimensions cannot exceed backing grid dimensions.");
    }
}

void MazeGenerator::formShape(MazeState& maze, const MazeBuildConfig& config)
{
    switch (config.shape)
    {
        case MazeShape::Rectangle:
            formRectangle(maze, config);
            break;

        case MazeShape::Ellipse:
            formEllipse(maze, config);
            break;

        default:
            throw std::invalid_argument("Unsupported maze shape.");
    }
}

void MazeGenerator::formRectangle(MazeState& maze, const MazeBuildConfig& config)
{
    const int startX = (config.backingWidth - config.mazeWidth) / 2;
    const int startY = (config.backingHeight - config.mazeHeight) / 2;
    const int endX = startX + config.mazeWidth;
    const int endY = startY + config.mazeHeight;

    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            Cell& cell = maze.cellAt(x, y);
            cell.active = (x >= startX && x < endX && y >= startY && y < endY);
        }
    }
}

void MazeGenerator::formEllipse(MazeState& maze, const MazeBuildConfig& config)
{
    const int startX = (config.backingWidth - config.mazeWidth) / 2;
    const int startY = (config.backingHeight - config.mazeHeight) / 2;

    const double radiusX = static_cast<double>(config.mazeWidth) / 2.0;
    const double radiusY = static_cast<double>(config.mazeHeight) / 2.0;

    const double centerX = static_cast<double>(startX) + radiusX;
    const double centerY = static_cast<double>(startY) + radiusY;

    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            Cell& cell = maze.cellAt(x, y);

            const double dx = (static_cast<double>(x) + 0.5 - centerX) / radiusX;
            const double dy = (static_cast<double>(y) + 0.5 - centerY) / radiusY;

            cell.active = (dx * dx + dy * dy) <= 1.0;
        }
    }
}

void MazeGenerator::initializeClosedWalls(MazeState& maze)
{
    for (int y = 0; y < maze.height(); ++y)
    {
        for (int x = 0; x < maze.width(); ++x)
        {
            Cell& cell = maze.cellAt(x, y);

            cell.north = true;
            cell.east  = true;
            cell.south = true;
            cell.west  = true;
        }
    }
}

void MazeGenerator::applyWallAlgorithm(MazeState& maze, const MazeBuildConfig& config, std::vector<MazeStep>& steps)
{
    MazeCarver::carve(maze, config.algorithm, steps);
}

} // namespace minotaur::world