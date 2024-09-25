#include "gold_bag.h"

void GoldBag::Update() {}

void GoldBag::Render() {
    Point position = GetPosition();
    UpdateFalling(position);

    RenderGoldBag(position);
    if (!is_dropped_) RenderGold(position);
}

bool GoldBag::Move(Direction direction) {
    Point new_pos = GetNewPosition(direction);
    if (!is_dropped_) SetPosition(new_pos);
    else Delete();

    return is_dropped_;
}

void GoldBag::UpdateFalling(Point& position) {
    int current_time = glutGet(GLUT_ELAPSED_TIME) / domain::GOLD_BAG_UPDATE;
    if (current_time - last_update_time_ >= update_interval_) {
        last_update_time_ = current_time;
        Point updating_pos = { position.x,position.y + 1 };

        if (!CanMoveTo(updating_pos)) fall_height_ = 0;
        else {
            ++fall_height_;
            SetPosition(updating_pos);
        }

        CheckAndPullPlayer(updating_pos);
    }
}

void GoldBag::CheckAndPullPlayer(const Point& position) const {
    if (player_ && player_->GetPosition() == position) {
        player_->Drug();

        if (!IsUnderObstacle(position)) player_->SetPosition({ position.x, position.y + 1 });
        else player_->SetPosition(position);

        if (fall_height_ > 1 && IsUnderObstacle(position)) player_->TakeDamage();
    }
}

bool GoldBag::IsUnderObstacle(const Point& position) const {
    Point under_position = { position.x, position.y + 1 };
    if (under_position.y >= grid::HEIGHT) return true;
    return std::any_of(objects_.begin(), objects_.end(),
        [&under_position](const auto& obj) { return obj->GetPosition() == under_position; });
}

bool GoldBag::CanMoveTo(const Point& position) {
    if (player_ && player_->GetPosition() != position) player_->UnDrug();

    if (!IsPositionValid(position)) {
        if (fall_height_ > 1) is_dropped_ = true;
        return false;
    }

    if (IsBlockedByObstacle(position)) {
        if (fall_height_ > 1) is_dropped_ = true;
        return false;
    }

    return true;
}

bool GoldBag::IsPositionValid(const Point& position) const {
    return position.x >= 0 && position.x < grid::WIDTH && position.y >= 0 && position.y < grid::HEIGHT;
}

bool GoldBag::IsBlockedByObstacle(const Point& position) {
    return std::any_of(objects_.begin(), objects_.end(),
        [&position](const auto& obj) {
            return obj->GetPosition() == position &&
                (dynamic_cast<Earth*>(obj.get()) ||
                    dynamic_cast<Emerald*>(obj.get()) ||
                    dynamic_cast<GoldBag*>(obj.get()));
        });
}

Point GoldBag::GetNewPosition(Direction direction) const {
    Point pos = GetPosition();
    switch (direction) {
    case LEFT:  pos.x = pos.x > 0 ? pos.x - 1 : pos.x + 1; break;
    case RIGHT: pos.x = pos.x < grid::WIDTH - 1 ? pos.x + 1 : pos.x - 1; break;
    default: break;
    }
    return pos;
}

void GoldBag::RenderGoldBag(const Point& position) const {
    glColor3f(color::YELLOW.R, color::YELLOW.G, color::YELLOW.B);
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2,
                     position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    glRecti(center.x - domain::OBJECT_SIZE, center.y - domain::OBJECT_SIZE,
        center.x + domain::OBJECT_SIZE, center.y + domain::OBJECT_SIZE);
}

void GoldBag::RenderGold(const Point& position) const {
    glColor3f(color::BROWN.R, color::BROWN.G, color::BROWN.B);
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2,
                     position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    glRecti(center.x - domain::OBJECT_SIZE, center.y - domain::OBJECT_SIZE / 3,
        center.x + domain::OBJECT_SIZE, center.y + domain::OBJECT_SIZE);
}