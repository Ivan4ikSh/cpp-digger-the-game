#pragma once
#include "game_object.h"
#include <algorithm>
#include <memory>
#include <vector>

class Player : public GameObject {
public:
    Player() : lives_(std::make_unique<int>(domain::LIVES)) {}
    Player(int* lives) : lives_(lives) {}

    void Update() override;
    void Render() override;

    Direction GetDirection();
    void Move(Direction direction);

    void TakeDamage();
    void Drug();
    void UnDrug();
private:
    Direction direction_ = Direction::RIGHT;
    std::unique_ptr<int> lives_;
    bool is_druged_ = false;

    void RenderPlayer();
    void DrawDirectionTriangle();
    void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

    void MoveToNewPosition();
    bool CanMove(const Point& pos);
};
