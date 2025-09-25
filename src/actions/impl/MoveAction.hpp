#pragma once

#include "Action.hpp"
#include "world/Cell.hpp"

namespace minotaur::actions {

class MoveAction : public Action {
public:
    explicit MoveAction(const world::Cell& target);

    std::string name() const override;
    int costAP() const override;

    Result validate(const Entity& entity, const ActionContext& ctx) const override;
    Result apply(Entity& entity, ActionContext& ctx) override;

    const world::Cell& targetCell() const;

private:
    world::Cell target_;
};

} // namespace minotaur::actions
