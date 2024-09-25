#include "emerald.h"

void Emerald::Update() {
    Delete();
}

void Emerald::Render() {
    Point position = GetPosition();

    glColor3f(color::GREEN.R, color::GREEN.G, color::GREEN.B);
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2 , position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    glRecti(center.x - domain::OBJECT_SIZE, center.y - domain::OBJECT_SIZE, center.x + domain::OBJECT_SIZE, center.y + domain::OBJECT_SIZE);
}
