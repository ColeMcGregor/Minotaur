#include "world/MazeCarver.hpp"

#include <string>
#include <utility>
#include <vector>

#include "core/RNG.hpp"

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

    Direction indexToDirection(int index)
    {
        switch (index)
        {
            case 0: return Direction::North;
            case 1: return Direction::East;
            case 2: return Direction::South;
            case 3: return Direction::West;
            default: return Direction::North;
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

    struct FrontierEdge
    {
        int fromX = -1;
        int fromY = -1;
        int toX = -1;
        int toY = -1;
        Direction dir = Direction::North;
    };
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

        case MazeAlgorithm::TestMed:
            carveTestMed(maze, steps);
            break;

        case MazeAlgorithm::TestHard:
            carveTestHard(maze, steps);
            break;

        case MazeAlgorithm::DepthFirst:
            carveDepthFirst(maze, steps);
            break;

        case MazeAlgorithm::Prim:
            carvePrim(maze, steps);
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

void MazeCarver::carveTestMed(MazeState& maze, std::vector<MazeStep>& steps)
{
    int stepNumber = 1;

    {
        MazeStep start;
        start.stepNumber = stepNumber++;
        start.type = MazeStepType::Start;
        start.decision = "Begin TestMed carve.";
        start.mutation = "None";
        start.result = "Will carve one L-shape and one zigzag.";
        steps.push_back(start);
    }

    const int centerX = maze.width() / 2;
    const int centerY = maze.height() / 2;

    std::vector<CarveAction> actions = {
        // L-shape: east, east, south, south
        {centerX - 5, centerY - 3, Direction::East},
        {centerX - 4, centerY - 3, Direction::East},
        {centerX - 3, centerY - 3, Direction::South},
        {centerX - 3, centerY - 2, Direction::South},

        // zigzag: east, south, east, south, east
        {centerX + 1, centerY - 2, Direction::East},
        {centerX + 2, centerY - 2, Direction::South},
        {centerX + 2, centerY - 1, Direction::East},
        {centerX + 3, centerY - 1, Direction::South},
        {centerX + 3, centerY,     Direction::East}
    };

    for (const CarveAction& action : actions)
    {
        applyAction(maze, action, stepNumber, steps);
    }

    {
        MazeStep complete;
        complete.stepNumber = stepNumber++;
        complete.type = MazeStepType::Complete;
        complete.decision = "Finished TestMed carve.";
        complete.mutation = "All TestMed actions applied.";
        complete.result = "L-shape and zigzag test corridors opened.";
        steps.push_back(complete);
    }
}

void MazeCarver::carveTestHard(MazeState& maze, std::vector<MazeStep>& steps)
{
    int stepNumber = 1;

    {
        MazeStep start;
        start.stepNumber = stepNumber++;
        start.type = MazeStepType::Start;
        start.decision = "Begin TestHard carve.";
        start.mutation = "None";
        start.result = "Will carve one box and one cross.";
        steps.push_back(start);
    }

    const int centerX = maze.width() / 2;
    const int centerY = maze.height() / 2;

    std::vector<CarveAction> actions = {
        // box perimeter, starting at top-left
        {centerX - 6, centerY - 4, Direction::East},
        {centerX - 5, centerY - 4, Direction::East},
        {centerX - 4, centerY - 4, Direction::East},

        {centerX - 3, centerY - 4, Direction::South},
        {centerX - 3, centerY - 3, Direction::South},
        {centerX - 3, centerY - 2, Direction::South},

        {centerX - 3, centerY - 1, Direction::West},
        {centerX - 4, centerY - 1, Direction::West},
        {centerX - 5, centerY - 1, Direction::West},

        {centerX - 6, centerY - 1, Direction::North},
        {centerX - 6, centerY - 2, Direction::North},
        {centerX - 6, centerY - 3, Direction::North},

        // cross centered to the right
        {centerX + 3, centerY + 1, Direction::North},
        {centerX + 3, centerY,     Direction::North},

        {centerX + 3, centerY + 1, Direction::South},
        {centerX + 3, centerY + 2, Direction::South},

        {centerX + 3, centerY + 1, Direction::West},
        {centerX + 2, centerY + 1, Direction::West},

        {centerX + 3, centerY + 1, Direction::East},
        {centerX + 4, centerY + 1, Direction::East}
    };

    for (const CarveAction& action : actions)
    {
        applyAction(maze, action, stepNumber, steps);
    }

    {
        MazeStep complete;
        complete.stepNumber = stepNumber++;
        complete.type = MazeStepType::Complete;
        complete.decision = "Finished TestHard carve.";
        complete.mutation = "All TestHard actions applied.";
        complete.result = "Box and cross test corridors opened.";
        steps.push_back(complete);
    }
}

void MazeCarver::carveDepthFirst(MazeState& maze, std::vector<MazeStep>& steps)
{
    const int width = maze.width();
    const int height = maze.height();

    minotaur::RNG rng;

    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::vector<std::pair<int, int>> stack;

    int stepNumber = 1;

    int startX = -1;
    int startY = -1;

    for (int y = 0; y < height && startX == -1; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (maze.cellAt(x, y).active)
            {
                startX = x;
                startY = y;
                break;
            }
        }
    }

    if (startX == -1)
    {
        MazeStep step;
        step.stepNumber = stepNumber++;
        step.type = MazeStepType::Complete;
        step.decision = "No active cells found.";
        step.mutation = "None";
        step.result = "DFS aborted.";
        steps.push_back(step);
        return;
    }

    {
        MazeStep step;
        step.stepNumber = stepNumber++;
        step.type = MazeStepType::Start;
        step.x = startX;
        step.y = startY;
        step.decision = "Begin DFS at " + cellString(startX, startY);
        step.mutation = "Initialize stack and visited set.";
        step.result = "Starting traversal.";
        steps.push_back(step);
    }

    visited[startY][startX] = true;
    stack.emplace_back(startX, startY);

    while (!stack.empty())
    {
        const auto [cx, cy] = stack.back();

        const int startDirIndex = rng.uniformInt(0, 3);

        std::string candidateStr = "[";
        bool foundNext = false;

        int nx = -1;
        int ny = -1;
        Direction chosenDir = Direction::North;

        for (int offset = 0; offset < 4; ++offset)
        {
            const int dirIndex = (startDirIndex + offset) % 4;
            const Direction d = indexToDirection(dirIndex);

            int tx = -1;
            int ty = -1;
            nextCoords(cx, cy, d, tx, ty);

            bool validCandidate = true;

            if (tx < 0 || ty < 0 || tx >= width || ty >= height)
            {
                validCandidate = false;
            }
            else if (!maze.cellAt(tx, ty).active)
            {
                validCandidate = false;
            }
            else if (visited[ty][tx])
            {
                validCandidate = false;
            }

            candidateStr += directionString(d) + ":" + (validCandidate ? cellString(tx, ty) : "invalid");
            if (offset < 3) candidateStr += ", ";

            if (!foundNext && validCandidate)
            {
                foundNext = true;
                nx = tx;
                ny = ty;
                chosenDir = d;
            }
        }

        candidateStr += "]";

        if (!foundNext)
        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Backtrack;
            step.x = cx;
            step.y = cy;
            step.candidates = candidateStr;
            step.decision = "No valid unvisited neighbors.";
            step.mutation = "Pop stack.";
            step.result = "Backtracking.";
            steps.push_back(step);

            stack.pop_back();
            continue;
        }

        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Consider;
            step.x = cx;
            step.y = cy;
            step.x2 = nx;
            step.y2 = ny;
            step.candidates = candidateStr;
            step.decision = "Random start direction chosen; first valid direction was " + directionString(chosenDir);
            step.mutation = "None";
            step.result = "Proceeding to " + cellString(nx, ny);
            steps.push_back(step);
        }

        maze.openWall(cx, cy, chosenDir);

        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Accept;
            step.x = cx;
            step.y = cy;
            step.x2 = nx;
            step.y2 = ny;
            step.decision = "Open " + directionString(chosenDir) + " wall.";
            step.mutation = "openWall(" + cellString(cx, cy) + ")";
            step.result = "Connected to " + cellString(nx, ny);
            steps.push_back(step);
        }

        visited[ny][nx] = true;
        stack.emplace_back(nx, ny);

        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Visit;
            step.x = nx;
            step.y = ny;
            step.decision = "Visit new cell.";
            step.mutation = "Push to stack.";
            step.result = "Continuing DFS.";
            steps.push_back(step);
        }
    }

    {
        MazeStep step;
        step.stepNumber = stepNumber++;
        step.type = MazeStepType::Complete;
        step.decision = "DFS complete.";
        step.mutation = "None.";
        step.result = "All reachable cells carved.";
        steps.push_back(step);
    }
}

void MazeCarver::carvePrim(MazeState& maze, std::vector<MazeStep>& steps)
{
    const int width = maze.width();
    const int height = maze.height();

    minotaur::RNG rng;

    std::vector<std::vector<bool>> inMaze(height, std::vector<bool>(width, false));
    std::vector<FrontierEdge> frontier;

    int stepNumber = 1;

    int startX = -1;
    int startY = -1;

    // Find first active cell
    for (int y = 0; y < height && startX == -1; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (maze.cellAt(x, y).active)
            {
                startX = x;
                startY = y;
                break;
            }
        }
    }

    if (startX == -1)
    {
        MazeStep step;
        step.stepNumber = stepNumber++;
        step.type = MazeStepType::Complete;
        step.decision = "No active cells found.";
        step.mutation = "None";
        step.result = "Prim aborted.";
        steps.push_back(step);
        return;
    }

    auto addFrontierEdges = [&](int x, int y)
    {
        const Direction order[4] = {
            Direction::North,
            Direction::East,
            Direction::South,
            Direction::West
        };

        for (Direction d : order)
        {
            int nx = -1;
            int ny = -1;
            nextCoords(x, y, d, nx, ny);

            if (nx < 0 || ny < 0 || nx >= width || ny >= height)
                continue;

            const Cell& c = maze.cellAt(nx, ny);
            if (!c.active)
                continue;

            frontier.push_back({x, y, nx, ny, d});
        }
    };

    // Start
    inMaze[startY][startX] = true;
    addFrontierEdges(startX, startY);

    {
        MazeStep step;
        step.stepNumber = stepNumber++;
        step.type = MazeStepType::Start;
        step.x = startX;
        step.y = startY;
        step.candidates = "frontier size = " + std::to_string(frontier.size());
        step.decision = "Begin Prim at " + cellString(startX, startY);
        step.mutation = "Mark start cell in maze and seed frontier.";
        step.result = "Prim initialized.";
        steps.push_back(step);
    }

    while (!frontier.empty())
    {
        const int index = rng.uniformInt(0, static_cast<int>(frontier.size()) - 1);
        const FrontierEdge edge = frontier[index];
        frontier.erase(frontier.begin() + index);

        const bool fromIn = inMaze[edge.fromY][edge.fromX];
        const bool toIn = inMaze[edge.toY][edge.toX];

        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Consider;
            step.x = edge.fromX;
            step.y = edge.fromY;
            step.x2 = edge.toX;
            step.y2 = edge.toY;
            step.candidates = "frontier size = " + std::to_string(frontier.size());
            step.decision = "Consider edge " + cellString(edge.fromX, edge.fromY) + " -> " + cellString(edge.toX, edge.toY);
            step.mutation = "None";
            step.result = "Checking carved/un carved endpoints.";
            steps.push_back(step);
        }

        // Reject if both already in maze or both not in maze
        if ((fromIn && toIn) || (!fromIn && !toIn))
        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Reject;
            step.x = edge.fromX;
            step.y = edge.fromY;
            step.x2 = edge.toX;
            step.y2 = edge.toY;
            step.candidates = "frontier size = " + std::to_string(frontier.size());
            step.decision = (fromIn && toIn)
                ? "Reject edge: both endpoints already in maze."
                : "Reject edge: neither endpoint is in maze.";
            step.mutation = "None";
            step.result = "Edge discarded.";
            steps.push_back(step);
            continue;
        }

        int carveFromX = edge.fromX;
        int carveFromY = edge.fromY;
        int newX = edge.toX;
        int newY = edge.toY;
        Direction carveDir = edge.dir;

        if (!fromIn && toIn)
        {
            carveFromX = edge.toX;
            carveFromY = edge.toY;
            newX = edge.fromX;
            newY = edge.fromY;
            carveDir = MazeState::opposite(edge.dir);
        }

        maze.openWall(carveFromX, carveFromY, carveDir);
        inMaze[newY][newX] = true;
        addFrontierEdges(newX, newY);

        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Accept;
            step.x = carveFromX;
            step.y = carveFromY;
            step.x2 = newX;
            step.y2 = newY;
            step.candidates = "frontier size = " + std::to_string(frontier.size());
            step.decision = "Accept edge and open " + directionString(carveDir) + " wall.";
            step.mutation = "openWall(" + cellString(carveFromX, carveFromY) + ")";
            step.result = "Connected " + cellString(carveFromX, carveFromY) + " to " + cellString(newX, newY) + ".";
            steps.push_back(step);
        }

        {
            MazeStep step;
            step.stepNumber = stepNumber++;
            step.type = MazeStepType::Visit;
            step.x = newX;
            step.y = newY;
            step.candidates = "frontier size = " + std::to_string(frontier.size());
            step.decision = "Add new cell to carved region.";
            step.mutation = "Mark inMaze and add its frontier edges.";
            step.result = "Prim continues.";
            steps.push_back(step);
        }
    }

    {
        MazeStep step;
        step.stepNumber = stepNumber++;
        step.type = MazeStepType::Complete;
        step.decision = "Prim complete.";
        step.mutation = "None";
        step.result = "All reachable active cells carved.";
        steps.push_back(step);
    }
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