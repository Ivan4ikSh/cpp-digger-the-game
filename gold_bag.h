#pragma once
#include "game_object.h"
#include "player.h"
#include "earth.h"
#include "emerald.h"

#include <algorithm>
#include <memory>
#include <vector>

class GoldBag : public GameObject {
public:
    GoldBag() = default;
    GoldBag(Point position, const std::vector<std::unique_ptr<GameObject>>& objects, Player* player)
        : objects_(objects), player_(player)
        {SetPosition(position);}

    void Update() override;
    void Render() override;

    bool Move(Direction direction);
private:
    const std::vector<std::unique_ptr<GameObject>>& objects_;
    Player* player_;
    bool is_dropped_ = false;
    int fall_height_ = 0;
    double last_update_time_ = 0.0;
    double update_interval_ = 1.0;

    void UpdateFalling(Point& position);
    void CheckAndPullPlayer(const Point& position) const;

    Point GetNewPosition(Direction direction) const;

    bool CanMoveTo(const Point& position);
    bool IsUnderObstacle(const Point& position) const;
    bool IsPositionValid(const Point& position) const;
    bool IsBlockedByObstacle(const Point& position);

    void RenderGoldBag(const Point& position) const;
    void RenderGold(const Point& position) const;
};
