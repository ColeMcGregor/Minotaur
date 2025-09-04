#pragma once
#include <cstdint>
#include <random>

namespace minotaur
{

    /**
     * @brief A class that provides a psuedo-random number generator.
     * 
     * @details This class is utilized for all random number generation in the game.
     * This includes loot and battle mechanics.
     * 
     * @author @Cole_McGregor
     * @date 2025-08-26
     * @version 1.0.0
     */

class RNG
{
public:
    RNG();
    explicit RNG(std::uint64_t seedValue);

    // Seed the engine (deterministic sequences when you reuse the same seed, which is useful for testing).
    void seed(std::uint64_t s);

    // Inclusive integer range [lo, hi]. If lo > hi, the bounds are swapped, should print to console if swap is performed.
    int uniformInt(int lo, int hi);

    // Uniform real in [0.0f, 1.0f). Never returns 1.0f. (somehwere between 0 and 1, not inclusive of 1)
    float uniform01();

    // Roll 1d8 → {1..8}. (1-8) (simulates rolling a 8-sided die)
    int rollD8();

    // Roll Nd8 and return the SUM (e.g., quantity=2 → classic 2d8 bell curve). (simulates rolling any amount of 8-sided dice)
    int rollD8(int quantity);

private:
    std::mt19937 engine_;                          // PRNG core (used to provide the psuedo-random numbers)
    std::uniform_int_distribution<int> d8_;        // cached [1..8] distribution, just for speed
    std::uniform_real_distribution<float> unit_;   // cached [0.0f,1.0f), just for speed
};

} 
