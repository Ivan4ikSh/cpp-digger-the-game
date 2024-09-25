#pragma once
#include "game_object.h"

#include <algorithm>
#include <memory>
#include <vector>

class Earth : public GameObject {
public:
    Earth(Point pos) { SetPosition(pos); }
    void Update() override;
    void Render() override;
};
