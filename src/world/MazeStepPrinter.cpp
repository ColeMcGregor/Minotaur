#include "world/MazeStepPrinter.hpp"

#include <iostream>

namespace minotaur::world {

std::string MazeStepPrinter::toString(MazeStepType type)
{
    switch (type)
    {
        case MazeStepType::Start:     return "Start";
        case MazeStepType::Visit:     return "Visit";
        case MazeStepType::Consider:  return "Consider";
        case MazeStepType::Accept:    return "Accept";
        case MazeStepType::Reject:    return "Reject";
        case MazeStepType::Backtrack: return "Backtrack";
        case MazeStepType::Complete:  return "Complete";
        default:                      return "Unknown";
    }
}

void MazeStepPrinter::printStep(const MazeStep& step)
{
    std::cout << "STEP " << step.stepNumber << "\n";
    std::cout << "Type: " << toString(step.type) << "\n";

    if (step.x >= 0 && step.y >= 0)
    {
        std::cout << "Current: (" << step.x << "," << step.y << ")";
        if (step.x2 >= 0 && step.y2 >= 0)
        {
            std::cout << " -> (" << step.x2 << "," << step.y2 << ")";
        }
        std::cout << "\n";
    }
    else
    {
        std::cout << "Current: N/A\n";
    }

    std::cout << "Candidates: " << (step.candidates.empty() ? "N/A" : step.candidates) << "\n";
    std::cout << "Decision: " << (step.decision.empty() ? "N/A" : step.decision) << "\n";
    std::cout << "Mutation: " << (step.mutation.empty() ? "N/A" : step.mutation) << "\n";
    std::cout << "Result: " << (step.result.empty() ? "N/A" : step.result) << "\n";
    std::cout << "----------------------------------------\n";
}

} // namespace minotaur::world