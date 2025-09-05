#include "world/MazeState.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>

/**
 * @brief The MazeState class represents the state of the maze, also holds the cells of the maze
 * @author @Cole_McGregor
 * @date 2025-09-04
 * @version 1.0.0
 * @copyright Copyright (c) 2025 Cole McGregor
 */
namespace minotaur::world
{

namespace
{

/**
 * @brief Makes a cell at the given coordinates
 * @param x The x coordinate of the cell
 * @param y The y coordinate of the cell
 * @return The cell at the given coordinates
 */
inline Cell makeCell(int x, int y)
{
    //make the cell reference, from Cell.hpp
    Cell c;
    //set the coordinates
    c.x = x;
    c.y = y;
    //set the walls to false
    c.north = false;
    c.east  = false;
    c.south = false;
    c.west  = false;
    //set the neighbors to false, and their locations to -1,-1
    for (int i = 0; i < 4; ++i)
    {
        c.nbr[i] = Neighbor{-1, -1, false};
    }
    //set the neighbor count to 0
    c.nbrCount = 0;
    //return the cell
    return c;
}
/**
 * @brief Gets a reference to the wall of the cell
 * @param c The cell
 * @param d The direction of the wall
 * @return A reference to the wall of the cell
 */
inline bool& wallRef(Cell& c, Direction d)
{
    //switch on the direction
    switch (d)
    {
        //return the north wall
        case Direction::North: return c.north;
        //return the east wall
        case Direction::East:  return c.east;
        //return the south wall
        case Direction::South: return c.south;
        //return the west wall
        case Direction::West:  return c.west;
    }
    return c.north; // unreachable, quiets some compilers that require a return outside of a switch statement
}

}

// ---------- MazeState implementation ----------

//default constructor
MazeState::MazeState() = default;

//make from a vector of ints, variable input accepted to make square, rectangle, or ragged
MazeState MazeState::generate(const std::vector<int>& specifications)
{
    //if the specifications is empty, throw an invalid argument exception
    if (specifications.empty())
    {
        throw std::invalid_argument("MazeState::generate: specification is empty");
    }

    MazeState mzst; //make a new maze state reference, call it mzst

    //-------------------------------------------------------------SQUARE-------------------------------------------------------------
    if (specifications.size() == 1)
    {
        // [N] square N x N
        int n = specifications[0];
        if (n < 0)
        {
            throw std::invalid_argument("MazeState::generate: n is less than 0, A Square must have an area to exist");
        }

        mzst.grid_.resize(static_cast<std::size_t>(n));
        for (int y = 0; y < n; ++y)
        {
            auto& row = mzst.grid_[static_cast<std::size_t>(y)];
            row.reserve(static_cast<std::size_t>(n));
            for (int x = 0; x < n; ++x)
            {
                row.push_back(makeCell(x, y));
            }
        }

        // boundary walls
        for (int y = 0; y < n; ++y)
        {
            for (int x = 0; x < n; ++x)
            {
                Cell& c = mzst.grid_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
                c.north = (y == 0);                                //if on the first row, the north wall is true
                c.south = (y == n - 1);                            //if on the last row, the south wall is true
                c.west  = (x == 0);                                //if on the first column, the west wall is true
                c.east  = (x == n - 1);                            //if on the last column, the east wall is true
            }
        }

        mzst.finalizeNeighbors();
        return mzst;
    }

    //-------------------------------------------------------------RECTANGLE-------------------------------------------------------------
    if (specifications.size() == 2)
    {
        // [rows, cols] rectangle
        int rows = specifications[0];
        int cols = specifications[1];
        if (rows < 0)
        {
            throw std::invalid_argument("MazeState::generate: rows is less than 0");
        }
        if (cols < 0)
        {
            throw std::invalid_argument("MazeState::generate: cols is less than 0");
        }

        mzst.grid_.resize(static_cast<std::size_t>(rows));
        for (int y = 0; y < rows; ++y)
        {
            auto& row = mzst.grid_[static_cast<std::size_t>(y)];
            row.reserve(static_cast<std::size_t>(cols));
            for (int x = 0; x < cols; ++x)
            {
                row.push_back(makeCell(x, y));
            }
        }

        // boundary walls
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                Cell& c = mzst.grid_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
                c.north = (y == 0);                               //if on the first row, the north wall is true
                c.south = (y == rows - 1);                         //if on the last row, the south wall is true
                c.west  = (x == 0);                                //if on the first column, the west wall is true
                c.east  = (x == cols - 1);                         //if on the last column, the east wall is true
            }
        }

        mzst.finalizeNeighbors();
        return mzst;
    }

    //-------------------------------------------------------------RAGGED-------------------------------------------------------------
    // [rows, len0, len1, ..., len{rows-1}]
    int rows = specifications[0];
    if (rows < 0)
    {
        rows = 0;
    }
    //TODO: check if this is corrrect, might need to be rows instead of rows + 1
    if (specifications.size() < static_cast<std::size_t>(rows + 1)) {
                throw std::invalid_argument("MazeState::generate: ragged requires rows+1 entries");
    }

    mzst.grid_.resize(static_cast<std::size_t>(rows));

    for (int y = 0; y < rows; ++y)
    {
        int cols = specifications[1 + y];
        if (cols < 0)
        {
            cols = 0;
        }

        auto& row = mzst.grid_[static_cast<std::size_t>(y)];
        row.reserve(static_cast<std::size_t>(cols));
        for (int x = 0; x < cols; ++x)
        {
            row.push_back(makeCell(x, y));
        }
    }
    /**
     * @brief Checks if a neighbor exists in the given direction, not a different function, but a lambda function still inside generate
     * 
     * @param x The x coordinate of the cell
     * @param y The y coordinate of the cell
     * @param d The direction to check
     * @return True if a neighbor exists in the given direction, false otherwise
     * @Warning: hasNeighbor WILL ONLY WORK BELOW, AND NOWHERE ELSE
     */
    auto hasNeighbor = [&mzst](int x, int y, Direction d) -> bool
    {
        switch (d)
        {
            //check if the north neighbor exists
            case Direction::North:
            {
                //get the y coordinate of the north neighbor
                const int ny = y - 1;
                //check if the north neighbor exists
                return ny >= 0 //check if the north neighbor is in bounds
                    && ny < static_cast<int>(mzst.grid_.size())
                    && x >= 0 //check if the x coordinate of the north neighbor is in bounds
                    && x < static_cast<int>(mzst.grid_[static_cast<std::size_t>(ny)].size());
            }
            //check if the south neighbor exists
            case Direction::South:
            {
                //get the y coordinate of the south neighbor
                const int ny = y + 1;
                //check if the south neighbor exists
                return ny >= 0 //check if the south neighbor is in bounds
                    && ny < static_cast<int>(mzst.grid_.size())
                    && x >= 0 //check if the x coordinate of the south neighbor is in bounds
                    && x < static_cast<int>(mzst.grid_[static_cast<std::size_t>(ny)].size());
            }
            //check if the east neighbor exists
            case Direction::East:
            {
                //get the y coordinate of the east neighbor
                return y >= 0 //check if the east neighbor is in bounds
                    && y < static_cast<int>(mzst.grid_.size())
                    && (x + 1) >= 0 //check if the x coordinate of the east neighbor is in bounds
                    && (x + 1) < static_cast<int>(mzst.grid_[static_cast<std::size_t>(y)].size());
            }
            //check if the west neighbor exists
            case Direction::West:
            {
                //get the y coordinate of the west neighbor
                return y >= 0 //check if the west neighbor is in bounds
                    && y < static_cast<int>(mzst.grid_.size())
                    && (x - 1) >= 0 //check if the x coordinate of the west neighbor is in bounds
                    && (x - 1) < static_cast<int>(mzst.grid_[static_cast<std::size_t>(y)].size());
            }
        }
        return false; //return false if the neighbor does not exist, but every cell should have at least one neighbor, as no cell should be isolated
    };

    /**
     * @brief this portion of the code will walls off the outside of the maze. 
     * it also causes the whole maze to be a walled off room, so this happens BEFORE the walls are finalized for the maze
     */
    //loop through each row in the maze
    for (int y = 0; y < rows; ++y)
    {
        const int cols = static_cast<int>(mzst.grid_[static_cast<std::size_t>(y)].size());
        //loop through each cell in the row
        for (int x = 0; x < cols; ++x)
        {
            //get the cell at the given coordinates
            Cell& c = mzst.grid_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
            //check neighbors, and set the walls to true if there isnt a neighbor in the given direction
            c.north = !hasNeighbor(x, y, Direction::North); 
            c.south = !hasNeighbor(x, y, Direction::South);
            c.west  = !hasNeighbor(x, y, Direction::West);
            c.east  = !hasNeighbor(x, y, Direction::East);
        }
    }
    //finalize the neighbors
    mzst.finalizeNeighbors();
    //return the maze state
    return mzst;
}

/**
 * @brief Makes a maze state from an initializer list of ints
 * @note this is a wrapper for the generate function that takes a vector of ints
 * @param specifications The initializer list of ints
 * @return The maze state
 */
MazeState MazeState::generate(std::initializer_list<int> specifications)
{
    return generate(std::vector<int>{specifications});
}

/**
 * @brief Gets the number of rows in the maze
 * @return The number of rows in the maze
 */
int MazeState::rows() const
{
    return static_cast<int>(grid_.size());
}

/**
 * @brief Gets the number of columns in the maze at the given row
 * @param row The row to get the number of columns at
 * @return The number of columns in the maze at the given row
 */
int MazeState::colsAtRow(int row) const
{
    if (!inBoundsRow(row))
    {
        return 0;
    }
    return static_cast<int>(grid_[static_cast<std::size_t>(row)].size());
}

/**
 * @brief Checks if the row is in bounds
 * @param row The row to check
 * @return True if the row is in bounds, false otherwise
 */
bool MazeState::inBoundsRow(int row) const
{
    return row >= 0 && row < static_cast<int>(grid_.size());
}

/**
 * @brief Checks if the coordinates of the cellare in bounds
 * @param x The x coordinate to check
 * @param y The y coordinate to check
 * @return True if the coordinates are in bounds, false otherwise
 */
bool MazeState::inBounds(int x, int y) const
{
    return y >= 0
        && y < static_cast<int>(grid_.size())
        && x >= 0
        && x < static_cast<int>(grid_[static_cast<std::size_t>(y)].size());
}

/**
 * @brief Checks if the cell is in bounds
 * @param x The x coordinate to check
 * @param y The y coordinate to check
 * @return True if the cell is in bounds, false otherwise
 */
bool MazeState::hasCell(int x, int y) const
{
    return inBounds(x, y);
}

/**
 * @brief Gets a reference to the cell at the given coordinates
 * @param x The x coordinate to get the cell at
 * @param y The y coordinate to get the cell at
 * @return A reference to the cell at the given coordinates, allows READ/WRITE access to the cell
 */
Cell& MazeState::cellAt(int x, int y)
{
    return grid_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
}

/**
 * @brief Gets a const reference to the cell at the given coordinates
 * @param x The x coordinate to get the cell at
 * @param y The y coordinate to get the cell at
 * @return A const reference to the cell at the given coordinates, allows READ ONLY access to the cell
 */
const Cell& MazeState::cellAt(int x, int y) const
{
    return grid_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
}

/**
 * @brief Gets the opposite direction
 * @param d The direction to get the opposite of
 * @return The opposite direction
 */
Direction MazeState::opposite(Direction d)
{
    switch (d)
    {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East:  return Direction::West;
        case Direction::West:  return Direction::East;
    }
    return Direction::North; // unreachable, quiets some compilers that require a return outside of a switch statement
}

void MazeState::finalizeNeighbors()
{
    //loop through each cell in the maze
    for (int y = 0; y < rows(); ++y)
    {
        //get the number of columns(and therefore cells) in the row
        const int cols = colsAtRow(y);
        //loop through each cell in the row
        for (int x = 0; x < cols; ++x)
        {
            //get the cell at the given coordinates
            Cell& c = grid_[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];

            //initialize the neighbor count
            int count = 0;

            // N
            //set the x and y coordinates of the north neighbor
            c.nbr[0].x = x;
            c.nbr[0].y = y - 1;
            c.nbr[0].valid = inBounds(c.nbr[0].x, c.nbr[0].y); //check if the north neighbor is in bounds, if it is it exists and can be added
            if (c.nbr[0].valid) { ++count; } //if the north neighbor is in bounds, increment the neighbor count

            // E
            c.nbr[1].x = x + 1; //set the x and y coordinates of the east neighbor
            c.nbr[1].y = y; 
            c.nbr[1].valid = inBounds(c.nbr[1].x, c.nbr[1].y); //check if the east neighbor is in bounds, if it is it exists and can be added
            if (c.nbr[1].valid) { ++count; }

            // S
            c.nbr[2].x = x; //set the x and y coordinates of the south neighbor 
            c.nbr[2].y = y + 1;
            c.nbr[2].valid = inBounds(c.nbr[2].x, c.nbr[2].y); //check if the south neighbor is in bounds, if it is it exists and can be added
            if (c.nbr[2].valid) { ++count; } //if the south neighbor is in bounds, increment the neighbor count

            // W
            c.nbr[3].x = x - 1;
            c.nbr[3].y = y;
            c.nbr[3].valid = inBounds(c.nbr[3].x, c.nbr[3].y); //check if the west neighbor is in bounds, if it is it exists and can be added
            if (c.nbr[3].valid) { ++count; } //if the west neighbor is in bounds, increment the neighbor count


            //a print statemennt for showing a neighbor count for each cell, used for testing and debugging
            std::cout << "Neighbor count after finalizing cell at (" << x << ", " << y << "): " << count << std::endl;
            c.nbrCount = count; //set the neighbor count to the count of valid neighbors
        }
    }
}

/**
 * @brief Links two cells together by opening the wall between them
 * @param x The x coordinate of the cell
 * @param y The y coordinate of the cell
 * @param d The direction of the wall to link
 * @note this will open the wall on both sides of the cells
 */
void MazeState::link(int x, int y, Direction d)
{
    //check if the cell is not in bounds
    if (!inBounds(x, y))
    {
        //then return because the cell is not in bounds and therefore no wall can be linked
        return;
    }

    //get the cell at the given coordinates
    Cell& a = cellAt(x, y);
    //get the reference to the wall in the given direction
    bool& wa = wallRef(a, d);
    //check if the wall is not already open
    if (!wa)
    {
        //then return because the wall is already open and therefore no wall can be linked
        return;
    }
    wa = false; //set the wall to false, meaning it is now open

    // mirror to neighbor if it exists
    const Neighbor nb = [this, &a, d]() -> Neighbor
    {
        switch (d)
        {
            case Direction::North: return a.nbr[0];
            case Direction::East:  return a.nbr[1];
            case Direction::South: return a.nbr[2];
            case Direction::West:  return a.nbr[3];
        }
        return Neighbor{0, 0, false}; //return a neighbor with x and y coordinates of 0 and a valid flag of false, to show the neighbor does not exist
    }();

    if (nb.valid) //check if the neighbor is valid
    {
        Cell& b = cellAt(nb.x, nb.y); //get the cell at the given coordinates
        bool& wb = wallRef(b, opposite(d)); //get the reference to the wall in the opposite direction
        wb = false; //set the wall to false, meaning it is now open
    }
}

/**
 * @brief Unlinks two cells by closing the wall between them
 * @param x The x coordinate of the cell
 * @param y The y coordinate of the cell
 * @param d The direction of the wall to unlink
 * @note this will close the wall on both sides of the cells
 */
void MazeState::unlink(int x, int y, Direction d)
{
    //check if the cell is not in bounds
    if (!inBounds(x, y))
    {
        //then return because the cell is not in bounds and therefore no wall can be unlinked
        return;
    }

    //get the cell at the given coordinates
    Cell& a = cellAt(x, y);
    //get the reference to the wall in the given direction
    bool& wa = wallRef(a, d);
    //check if the wall is not already closed
    if (wa)
    {
        //then return because the wall is already closed and therefore no wall can be unlinked
        return;
    }
    wa = true; //set the wall to true, meaning it is now closed

    //get the neighbor in the given direction, using a lambda function to return the neighbor
    const Neighbor nb = [this, &a, d]() -> Neighbor
    {
        switch (d)
        {
            case Direction::North: return a.nbr[0];
            case Direction::East:  return a.nbr[1];
            case Direction::South: return a.nbr[2];
            case Direction::West:  return a.nbr[3];
        }
        return Neighbor{0, 0, false};
    }();

    if (nb.valid) //check if the neighbor is valid, therefore exists and can be unlinked
    {
        Cell& b = cellAt(nb.x, nb.y); //get the cell at the given coordinates
        bool& wb = wallRef(b, opposite(d)); //get the reference to the wall in the opposite direction
        wb = true; //set the wall to true, meaning it is now closed
    }
}

/**
 * @brief Toggles the wall between two cells by flipping the wall state
 * @param x The x coordinate of the cell
 * @param y The y coordinate of the cell
 * @param d The direction of the wall to toggle
 * @note this will toggle the wall on both sides of the cells
 */
void MazeState::toggleWall(int x, int y, Direction d)
{
    //check if the cell is not in bounds
    if (!inBounds(x, y))
    {
        //then return because the cell is not in bounds and therefore no wall can be toggled
        return;
    }

    //get the cell at the given coordinates
    Cell& a = cellAt(x, y);
    //get the reference to the wall in the given direction
    bool& wa = wallRef(a, d);
    const bool newVal = !wa; //get the new value of the wall by flipping the current state
    wa = newVal;

    //get the neighbor in the given direction, using a lambda function to return the neighbor
    const Neighbor nb = [this, &a, d]() -> Neighbor
    {
        switch (d)
        {
            case Direction::North: return a.nbr[0];
            case Direction::East:  return a.nbr[1];
            case Direction::South: return a.nbr[2];
            case Direction::West:  return a.nbr[3];
        }
        return Neighbor{0, 0, false}; //return a neighbor with x and y coordinates of 0 and a valid flag of false, to show the neighbor does not exist
    }();

    if (nb.valid) //check if the neighbor is valid, therefore exists and can be toggled
    {
        Cell& b = cellAt(nb.x, nb.y); //get the cell at the given coordinates
        bool& wb = wallRef(b, opposite(d)); //get the reference to the wall in the opposite direction
        wb = newVal; //set the wall to the new value
    }
}

} 
