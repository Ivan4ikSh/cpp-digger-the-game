#include "player.h"

void Player::Update() {}

void Player::Render() {
    RenderPlayer();
    DrawDirectionTriangle();
}

Direction Player::GetDirection() { return direction_; }

void Player::Move(Direction direction) {
    direction_ = direction;
    MoveToNewPosition();
}

void Player::TakeDamage() {
    if (*lives_ > 0) --(*lives_);
}

void Player::Drug() { is_druged_ = true; }
void Player::UnDrug() { is_druged_ = false; }

void Player::RenderPlayer() {
    Point position = GetPosition();
    glColor3f(color::BLUE.R, color::BLUE.G, color::BLUE.B);
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2,
                     position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    glRecti(center.x - domain::OBJECT_SIZE, center.y - domain::OBJECT_SIZE,
        center.x + domain::OBJECT_SIZE, center.y + domain::OBJECT_SIZE);
}

void Player::DrawDirectionTriangle() {
    Point position = GetPosition();
    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2,
                     position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    float half_size = domain::OBJECT_SIZE;

    glColor3f(color::PINK.R, color::PINK.G, color::PINK.B);
    glBegin(GL_TRIANGLES);
    switch (direction_) {
    case Direction::UP:
        DrawTriangle(center.x, center.y - domain::OBJECT_SIZE,
            center.x - half_size, center.y,
            center.x + half_size, center.y);
        break;
    case Direction::DOWN:
        DrawTriangle(center.x, center.y + domain::OBJECT_SIZE,
            center.x - half_size, center.y,
            center.x + half_size, center.y);
        break;
    case Direction::LEFT:
        DrawTriangle(center.x - domain::OBJECT_SIZE, center.y,
            center.x, center.y - half_size,
            center.x, center.y + half_size);
        break;
    case Direction::RIGHT:
        DrawTriangle(center.x + domain::OBJECT_SIZE, center.y,
            center.x, center.y - half_size,
            center.x, center.y + half_size);
        break;
    }
    glEnd();
}

void Player::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
}

bool Player::CanMove(const Point& pos) {
    return pos.x >= 0 && pos.x < grid::WIDTH && pos.y >= 0 && pos.y < grid::HEIGHT;
}

void Player::MoveToNewPosition() {
    Point position = GetPosition();
    Point new_position = position;

    switch (direction_) {
    case UP:    new_position.y -= is_druged_ ? 0 : 1; break;
    case DOWN:  new_position.y += is_druged_ ? 0 : 1; break;
    case LEFT:  new_position.x -= 1; break;
    case RIGHT: new_position.x += 1; break;
    default:    break;
    }

    if (CanMove(new_position)) SetPosition(new_position);
}
