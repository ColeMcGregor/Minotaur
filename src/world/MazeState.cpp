#include "world/MazeState.hpp"

namespace minotaur::world {

// ---------- helpers ----------

namespace
{
inline bool& wallRef(Cell& c, Direction d)
{
    switch (d)
    {
        case Direction::North: return c.north;
        case Direction::East:  return c.east;
        case Direction::South: return c.south;
        case Direction::West:  return c.west;
    }
    return c.north;
}
}

// ---------- MazeState implementation ----------

MazeState::MazeState(int width, int height)
    : width_(width), height_(height), grid_(static_cast<std::size_t>(width * height))
{
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            Cell& c = grid_[static_cast<std::size_t>(index(x, y))];
            c.x = x;
            c.y = y;
            c.active = false;

            c.north = true;
            c.south = true;
            c.east  = true;
            c.west  = true;
        }
    }
}

// ---------- dimensions ----------

int MazeState::width() const
{
    return width_;
}

int MazeState::height() const
{
    return height_;
}

// ---------- bounds / existence ----------

bool MazeState::inBounds(int x, int y) const
{
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool MazeState::hasCell(int x, int y) const
{
    return inBounds(x, y) && cellAt(x, y).active;
}

// ---------- access ----------

Cell& MazeState::cellAt(int x, int y)
{
    return grid_[static_cast<std::size_t>(index(x, y))];
}

const Cell& MazeState::cellAt(int x, int y) const
{
    return grid_[static_cast<std::size_t>(index(x, y))];
}

// ---------- adjacency ----------

void MazeState::finalizeNeighbors()
{
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            Cell& c = cellAt(x, y);

            c.nbrCount = 0;

            if (!c.active)
            {
                continue;
            }

            // N
            c.nbr[0] = {x, y - 1, Direction::North, hasCell(x, y - 1)};
            if (c.nbr[0].valid) ++c.nbrCount;

            // E
            c.nbr[1] = {x + 1, y, Direction::East, hasCell(x + 1, y)};
            if (c.nbr[1].valid) ++c.nbrCount;

            // S
            c.nbr[2] = {x, y + 1, Direction::South, hasCell(x, y + 1)};
            if (c.nbr[2].valid) ++c.nbrCount;

            // W
            c.nbr[3] = {x - 1, y, Direction::West, hasCell(x - 1, y)};
            if (c.nbr[3].valid) ++c.nbrCount;
        }
    }
}

// ---------- wall ops ----------

void MazeState::openWall(int x, int y, Direction d)
{
    if (!hasCell(x, y)) return;

    Cell& a = cellAt(x, y);
    bool& wa = wallRef(a, d);

    if (!wa) return;
    wa = false;

    const Neighbor& nb =
        (d == Direction::North) ? a.nbr[0] :
        (d == Direction::East)  ? a.nbr[1] :
        (d == Direction::South) ? a.nbr[2] :
                                 a.nbr[3];

    if (nb.valid)
    {
        Cell& b = cellAt(nb.x, nb.y);
        wallRef(b, opposite(d)) = false;
    }
}

void MazeState::closeWall(int x, int y, Direction d)
{
    if (!hasCell(x, y)) return;

    Cell& a = cellAt(x, y);
    bool& wa = wallRef(a, d);

    if (wa) return;
    wa = true;

    const Neighbor& nb =
        (d == Direction::North) ? a.nbr[0] :
        (d == Direction::East)  ? a.nbr[1] :
        (d == Direction::South) ? a.nbr[2] :
                                 a.nbr[3];

    if (nb.valid)
    {
        Cell& b = cellAt(nb.x, nb.y);
        wallRef(b, opposite(d)) = true;
    }
}

void MazeState::toggleWall(int x, int y, Direction d)
{
    if (!hasCell(x, y)) return;

    Cell& a = cellAt(x, y);
    bool& wa = wallRef(a, d);

    const bool newVal = !wa;
    wa = newVal;

    const Neighbor& nb =
        (d == Direction::North) ? a.nbr[0] :
        (d == Direction::East)  ? a.nbr[1] :
        (d == Direction::South) ? a.nbr[2] :
                                 a.nbr[3];

    if (nb.valid)
    {
        Cell& b = cellAt(nb.x, nb.y);
        wallRef(b, opposite(d)) = newVal;
    }
}

// ---------- utility ----------

Direction MazeState::opposite(Direction d)
{
    switch (d)
    {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East:  return Direction::West;
        case Direction::West:  return Direction::East;
    }
    return Direction::North;
}

// ---------- private ----------

int MazeState::index(int x, int y) const
{
    return y * width_ + x;
}

} // namespace minotaur::world