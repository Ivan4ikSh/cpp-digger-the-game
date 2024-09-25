#pragma once
#include "game_object.h"

class Emerald : public GameObject {
public:
    Emerald(Point start_pos) { SetPosition(start_pos); }
    void Update() override;
    void Render() override;
};