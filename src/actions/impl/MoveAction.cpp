#include "impl/MoveAction.hpp"
#include "components/Position.hpp"
#include <sstream>

namespace minotaur::actions {

MoveAction::MoveAction(const world::Cell& target)
    : target_(target) {}

std::string MoveAction::name() const {
    return "Move";
}

int MoveAction::costAP() const {
    return 1;
}

const world::Cell& MoveAction::targetCell() const {
    return target_;
}

Result MoveAction::validate(const Entity& entity, const ActionContext& ctx) const {
    if (!ctx.maze.inBounds(target_.x, target_.y)) {
        return Result::invalid("Target cell is out of bounds.");
    }

    const auto& pos = ctx.store.get<components::Position>(entity.id);

    int dx = std::abs(pos.cell.x - target_.x);
    int dy = std::abs(pos.cell.y - target_.y);
    if ((dx + dy) != 1) {
        return Result::invalid("Can only move to adjacent cells.");
    }

    const auto& targetCell = ctx.maze.cellAt(target_.x, target_.y);
    const auto& sourceCell = ctx.maze.cellAt(pos.cell.x, pos.cell.y);

    if (target_.x > pos.cell.x && sourceCell.east)   return Result::invalid("Wall blocks east.");
    if (target_.x < pos.cell.x && sourceCell.west)   return Result::invalid("Wall blocks west.");
    if (target_.y > pos.cell.y && sourceCell.south)  return Result::invalid("Wall blocks south.");
    if (target_.y < pos.cell.y && sourceCell.north)  return Result::invalid("Wall blocks north.");

    return Result::success("Move is valid.");
}

Result MoveAction::apply(Entity& entity, ActionContext& ctx) {
    auto& pos = ctx.store.get<components::Position>(entity.id);
    pos.cell = target_;

    std::ostringstream oss;
    oss << "Moved to (" << target_.x << ", " << target_.y << ")";
    return Result::success(oss.str());
}

} // namespace minotaur::actions
