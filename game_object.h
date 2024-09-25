#pragma once
#include "domain.h"

class GameObject {
public:
    GameObject() = default;

    virtual void Update() = 0;
    virtual void Render() = 0;

    Point GetPosition() const { return position_; }
    void SetPosition(Point pos) { this->position_ = pos; }
    void Delete() { this->position_ = { -1,-1 }; }
private:
    Point position_ = { 7, 9 };
};