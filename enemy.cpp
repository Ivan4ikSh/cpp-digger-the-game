#include "enemy.h"

void Enemy::Update() {
    int current_time = glutGet(GLUT_ELAPSED_TIME) / domain::enemy::UPDATE;

    if (is_player_catched_) {
        if (current_time - timing_.catch_time >= timing_.pause_duration) is_player_catched_ = false;
        else return;
    }

    if (current_time - timing_.last_update_time >= timing_.update_interval) {
        if (IsUnderGold()) Dead();

        timing_.last_update_time = current_time;
        if (!CanMoveForPlayer()) MoveRandomly();
        else Move();
    }
}

void Enemy::Render() {
    Point position = GetPosition();
    if (!is_player_catched_) glColor3f(color::RED.R, color::RED.G, color::RED.B);
    else glColor3f(color::PINK.R, color::PINK.G, color::PINK.B);

    Point center = { position.x * domain::CELL_SIZE + domain::CELL_SIZE / 2 , position.y * domain::CELL_SIZE + domain::CELL_SIZE / 2 };
    glRecti(center.x - domain::OBJECT_SIZE, center.y - domain::OBJECT_SIZE, center.x + domain::OBJECT_SIZE, center.y + domain::OBJECT_SIZE);
}

void Enemy::Dead() {
    Delete();
}

bool Enemy::IsWalkable(const Point& pos) {
    if (pos.x < 0 || pos.x >= grid::WIDTH || pos.y < 0 || pos.y >= grid::HEIGHT) return false;

    for (const auto& obj : objects_) {
        if (auto earth = dynamic_cast<Earth*>(obj.get())) {
            if (earth->GetPosition() == pos) return true;
        }

        if (obj->GetPosition() == pos) return false;
    }

    return true;
}

bool Enemy::IsWithinBounds(const Point& pos) {
    return pos.x >= 0 && pos.x < grid::WIDTH && pos.y >= 0 && pos.y < grid::HEIGHT;
}

bool Enemy::IsObstacle(const Point& pos) {
    for (const auto& obj : objects_) {
        if (obj->GetPosition() == pos && (dynamic_cast<Earth*>(obj.get()) || dynamic_cast<GoldBag*>(obj.get()))) return true;
    }
    return false;
}

bool Enemy::CanMoveForPlayer() {
    Point enemy_position = GetPosition();
    Point player_position = player_->GetPosition();

    if (enemy_position == player_position) return true;

    std::unordered_set<Point, PointHash> visited;
    std::queue<Point> queue;

    queue.push(enemy_position);
    visited.insert(enemy_position);

    const std::vector<Point> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop();

        for (const auto& direction : directions) {
            Point next = { current.x + direction.x, current.y + direction.y };

            if (IsWithinBounds(next) && !IsObstacle(next) && visited.find(next) == visited.end()) {
                if (next == player_position) return true;

                queue.push(next);
                visited.insert(next);
            }
        }
    }

    return false;
}

void Enemy::MoveRandomly() {
    std::vector<Point> directions = { {0, domain::enemy::SPEED}, {domain::enemy::SPEED, 0}, {0, -domain::enemy::SPEED}, {-domain::enemy::SPEED, 0} };

    std::random_shuffle(directions.begin(), directions.end());

    for (const auto& direction : directions) {
        Point newPosition = { GetPosition().x + direction.x, GetPosition().y + direction.y };

        if (IsValidPosition(newPosition)) {
            SetPosition(newPosition);
            break;
        }
    }
}

bool Enemy::IsValidPosition(const Point& pos) {
    if (pos.x < 0 || pos.x >= grid::WIDTH || pos.y < 0 || pos.y >= grid::HEIGHT) return false;

    for (const auto& obj : objects_) {
        if (obj->GetPosition() == pos) return false;
    }

    return true;
}

bool Enemy::IsUnderGold() {
    Point enemy_position = GetPosition();
    for (const auto& obj : objects_) {
        if (auto gold_bag = dynamic_cast<GoldBag*>(obj.get())) {
            if (gold_bag->GetPosition() == enemy_position) return true;
        }
    }
    return false;
}

void Enemy::Move() {
    Point enemy_position = GetPosition();
    Point player_position = player_->GetPosition();

    if (enemy_position == player_position) {
        is_player_catched_ = true;
        timing_.catch_time = glutGet(GLUT_ELAPSED_TIME) / domain::enemy::UPDATE;
        player_->TakeDamage();
        return;
    }

    std::unordered_map<Point, Point, PointHash> came_from;
    std::queue<Point> queue;
    queue.push(enemy_position);
    came_from[enemy_position] = enemy_position;

    const std::vector<Point> directions = { {0, domain::enemy::SPEED}, {domain::enemy::SPEED, 0}, {0, -domain::enemy::SPEED}, {-domain::enemy::SPEED, 0} };

    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop();

        if (current == player_position) break;

        for (const auto& direction : directions) {
            Point next = { current.x + direction.x, current.y + direction.y };

            if (IsWithinBounds(next) && !IsObstacle(next) && came_from.find(next) == came_from.end()) {
                queue.push(next);
                came_from[next] = current;
            }
        }
    }

    std::vector<Point> path;
    Point step = player_position;

    if (came_from.find(player_position) == came_from.end()) return;

    while (step != enemy_position) {
        path.push_back(step);
        step = came_from[step];
    }

    if (!path.empty()) SetPosition(path.back());
}

