#pragma once
#include <memory>
#include <string>
#include "rules/DecisionContext.hpp"
#include "actions/Action.hpp"   // for Action + Result + ActionPtr

namespace minotaur {

/**
 * Strategy interface for deciding the next Action.
 * Implementations: HumanController, HuntedAIController, MinotaurAIController, etc.
 */
class Controller {
public:
    virtual ~Controller() = default;

    /// Optional identifier for logs/UI (e.g., "Human", "HuntedAI").
    virtual std::string name() const { return "Controller"; }

    /// Return the next Action to attempt (nullptr means "pass / end turn").
    virtual ActionPtr chooseAction(const DecisionContext& ctx) = 0;
};

} // namespace minotaur
