#pragma once
#include "earth.h"
#include "game_object.h"
#include "gold_bag.h"
#include "player.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Enemy : public GameObject {
public:
    Enemy(Point pos, const std::vector<std::unique_ptr<GameObject>>& objects, Player* player)
        : objects_(objects), player_(player) {
        SetPosition(pos);
        srand(time(NULL));
    }

    void Update() override;
    void Render() override;
    void Dead();
private:
    struct Timing {
        double last_update_time = 0.0;
        double update_interval = 1.0;

        double catch_time = 0.0;
        double pause_duration = 5.0;
    };
    Timing timing_ = { 0.0,1.0,0.0,5.0 };

    const std::vector<std::unique_ptr<GameObject>>& objects_;
    Player* player_;
    bool is_player_catched_ = false;    

    struct Node {
        Point position;
        Node(Point pos) : position(pos) {}
    };
    
    void MoveRandomly();
    void Move();

    bool CanMoveForPlayer();

    bool IsWalkable(const Point& pos);
    bool IsWithinBounds(const Point& pos);
    bool IsObstacle(const Point& pos);
    bool IsValidPosition(const Point& pos);
    bool IsUnderGold();

};
