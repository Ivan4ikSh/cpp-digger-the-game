#include "earth.h"

void Earth::Update() {
    Delete();
}

void Earth::Render() {
    Point position = GetPosition();

    glColor3f(color::GRAY.R, color::GRAY.G, color::GRAY.B);
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2 , position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    glRecti(center.x - domain::CELL_SIZE + domain::EARTH_SIZE, center.y - domain::CELL_SIZE + domain::EARTH_SIZE, center.x + domain::CELL_SIZE - domain::EARTH_SIZE, center.y + domain::CELL_SIZE - domain::EARTH_SIZE);
};

