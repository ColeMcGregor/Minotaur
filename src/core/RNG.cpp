#include "RNG.hpp"
#include <algorithm>  // for std::minmax

namespace minotaur
{

/**
 * @brief C file for the RNG class.
 * 
 * @details This file contains the implementation of the RNG class.
 * 
 * @author @Cole_McGregor
 * @date 2025-08-26
 * @version 1.0.0
 */

 // Default constructor, uses a random seed
RNG::RNG()
    : engine_{std::random_device{}()}
    , d6_{1, 6}
    , unit_{0.0f, 1.0f}
{
}

// Constructor that takes a seed value, used for testing
RNG::RNG(std::uint64_t seedValue)
    : engine_{static_cast<std::mt19937::result_type>(seedValue)}
    , d6_{1, 6}
    , unit_{0.0f, 1.0f}
{
}

// Seed the engine (deterministic sequences when you reuse the same seed, which is useful for testing)
void RNG::seed(std::uint64_t s)
{
    engine_.seed(static_cast<std::mt19937::result_type>(s));
}

// Inclusive integer range [lo, hi]. If lo > hi, the bounds are swapped, should print to console if swap is performed.
int RNG::uniformInt(int lo, int hi)
{
    if (lo > hi)
    {
        std::fprintf(stderr, "[RNG] uniformInt: invalid range lo=%d hi=%d\n", lo, hi);
        std::exit(1);
    }
    // If the bounds are the same, return the bound
    if (lo == hi)
    {
        return lo;
    }
    // Create a distribution with the bounds
    std::uniform_int_distribution<int> dist{lo, hi};
    // Return the random number by feeding the engine the distribution
    return dist(engine_);
}

// Uniform real in [0.0f, 1.0f). Never returns 1.0f.
float RNG::uniform01()
{
    return unit_(engine_);
}

// Roll 1d6 → {1..6}.
int RNG::rollD6()
{
    return d6_(engine_);
}

// Roll Nd6 and return the SUM (e.g., quantity=2 → classic 2d6 bell curve).
int RNG::rollD6(int quantity)
{
    // If the quantity is less than or equal to 0, return 0
    if (quantity <= 0)
    {
        return 0;
    }

    // Sum the rolls
    int sum = 0;
    for (int i = 0; i < quantity; ++i)
    {
        sum += d6_(engine_);
    }
    // Return the sum
    return sum;
}

} 
