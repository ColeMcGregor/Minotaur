#include "world/MazeCarver.hpp"

namespace minotaur::world {

void MazeCarver::carve(MazeState& maze, MazeAlgorithm algorithm)
{
    switch (algorithm)
    {
        case MazeAlgorithm::None:
            carveNone(maze);
            break;

        case MazeAlgorithm::DepthFirst:
            carveDepthFirst(maze);
            break;

        default:
            carveNone(maze);
            break;
    }
}

void MazeCarver::carveNone(MazeState& maze)
{
    (void)maze;
}

void MazeCarver::carveDepthFirst(MazeState& maze)
{
    (void)maze;
}

} // namespace minotaur::world