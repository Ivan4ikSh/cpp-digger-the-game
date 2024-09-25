#include "bullet.h"

void Bullet::Update() {
    Point position = GetPosition();
    if (IsPositionValid(position)) {
        switch (direction_)
        {
        case UP:
            SetPosition({ position.x, position.y - 1 });
            break;
        case DOWN:
            SetPosition({ position.x, position.y + 1 });
            break;
        case LEFT:
            SetPosition({ position.x - 1, position.y });
            break;
        case RIGHT:
            SetPosition({ position.x + 1, position.y });
            break;
        default:
            break;
        }
    }
}

void Bullet::Render() {
    glColor3f(color::WHITE.R, color::WHITE.G, color::WHITE.B);
    Point position = GetPosition();
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2,
                     position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };

    float radius = domain::OBJECT_SIZE / 2.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center.x, center.y);
    for (int i = 0; i <= 20; ++i) {
        float angle = i * 2.0f * domain::PI / 20;
        float dx = cos(angle) * radius;
        float dy = sin(angle) * radius;
        glVertex2f(center.x + dx, center.y + dy);
    }
    glEnd();
}

bool Bullet::IsPositionValid(const Point& pos) const {
    return pos.x >= 0 && pos.x < grid::WIDTH && pos.y >= 0 && pos.y < grid::HEIGHT && !IsObstacle(pos);
}

bool Bullet::IsObstacle(const Point& pos) const {
    for (const auto& obj : objects_) {
        if (obj->GetPosition() == GetPosition()) return true;
    }
    return false;
}
