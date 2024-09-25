#pragma once
#include <memory>
#include <vector>

#include "domain.h"
#include "game_object.h"

class Bullet : public GameObject {
public:
    Bullet() = default;
	Bullet(Direction direction, Point position, const std::vector<std::unique_ptr<GameObject>>& objects)
        : direction_(direction), objects_(objects) {SetPosition(position);}

    void Update() override;
    void Render() override;

    bool IsPositionValid(const Point& pos) const;
private:
    const std::vector<std::unique_ptr<GameObject>>& objects_;
	Direction direction_ = Direction::LEFT;

    bool IsObstacle(const Point& pos) const;
};