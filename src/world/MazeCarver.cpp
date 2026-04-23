#include "world/MazeCarver.hpp"

#include <string>
#include <vector>

namespace minotaur::world {

namespace
{
    std::string cellString(int x, int y)
    {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }

    std::string directionString(Direction dir)
    {
        switch (dir)
        {
            case Direction::North: return "North";
            case Direction::East:  return "East";
            case Direction::South: return "South";
            case Direction::West:  return "West";
            default:               return "Unknown";
        }
    }

    bool nextCoords(int x, int y, Direction dir, int& outX, int& outY)
    {
        outX = x;
        outY = y;

        switch (dir)
        {
            case Direction::North: --outY; break;
            case Direction::East:  ++outX; break;
            case Direction::South: ++outY; break;
            case Direction::West:  --outX; break;
            default: return false;
        }

        return true;
    }
}

void MazeCarver::carve(MazeState& maze, MazeAlgorithm algorithm, std::vector<MazeStep>& steps)
{
    steps.clear();

    switch (algorithm)
    {
        case MazeAlgorithm::None:
            carveNone(maze, steps);
            break;

        case MazeAlgorithm::TestEasy:
            carveTestEasy(maze, steps);
            break;

        case MazeAlgorithm::DepthFirst:
            carveDepthFirst(maze, steps);
            break;

        default:
            carveNone(maze, steps);
            break;
    }
}

void MazeCarver::carveNone(MazeState& maze, std::vector<MazeStep>& steps)
{
    (void)maze;

    MazeStep step;
    step.stepNumber = 1;
    step.type = MazeStepType::Complete;
    step.decision = "No carving algorithm selected.";
    step.mutation = "None";
    step.result = "Maze remains fully closed.";
    steps.push_back(step);
}

void MazeCarver::carveTestEasy(MazeState& maze, std::vector<MazeStep>& steps)
{
    int stepNumber = 1;

    {
        MazeStep start;
        start.stepNumber = stepNumber++;
        start.type = MazeStepType::Start;
        start.decision = "Begin TestEasy carve.";
        start.mutation = "None";
        start.result = "Will carve one vertical line and one horizontal line.";
        steps.push_back(start);
    }

    const int centerX = maze.width() / 2;
    const int centerY = maze.height() / 2;

    std::vector<CarveAction> actions = {
        // vertical line
        {centerX - 4, centerY - 2, Direction::South},
        {centerX - 4, centerY - 1, Direction::South},
        {centerX - 4, centerY,     Direction::South},
        {centerX - 4, centerY + 1, Direction::South},

        // horizontal line
        {centerX - 2, centerY + 3, Direction::East},
        {centerX - 1, centerY + 3, Direction::East},
        {centerX,     centerY + 3, Direction::East},
        {centerX + 1, centerY + 3, Direction::East}
    };

    for (const CarveAction& action : actions)
    {
        applyAction(maze, action, stepNumber, steps);
    }

    {
        MazeStep complete;
        complete.stepNumber = stepNumber++;
        complete.type = MazeStepType::Complete;
        complete.decision = "Finished TestEasy carve.";
        complete.mutation = "All TestEasy actions applied.";
        complete.result = "Vertical and horizontal test corridors opened.";
        steps.push_back(complete);
    }
}

void MazeCarver::carveDepthFirst(MazeState& maze, std::vector<MazeStep>& steps)
{
    (void)maze;

    MazeStep step;
    step.stepNumber = 1;
    step.type = MazeStepType::Complete;
    step.decision = "DepthFirst not implemented yet.";
    step.mutation = "None";
    step.result = "Maze remains unchanged.";
    steps.push_back(step);
}

void MazeCarver::applyAction(MazeState& maze, const CarveAction& action, int& stepNumber, std::vector<MazeStep>& steps)
{
    int nx = -1;
    int ny = -1;
    nextCoords(action.x, action.y, action.dir, nx, ny);

    MazeStep step;
    step.stepNumber = stepNumber++;
    step.type = MazeStepType::Accept;
    step.x = action.x;
    step.y = action.y;
    step.x2 = nx;
    step.y2 = ny;
    step.candidates = "Fixed test action";
    step.decision = "Open " + directionString(action.dir) + " wall from " + cellString(action.x, action.y);
    step.mutation = "Call openWall on " + cellString(action.x, action.y);
    step.result = "Passage opened to " + cellString(nx, ny);

    maze.openWall(action.x, action.y, action.dir);
    steps.push_back(step);
}

} // namespace minotaur::world