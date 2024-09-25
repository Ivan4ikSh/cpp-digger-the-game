#include "game.h"

Game::Game() {
    InitLevel(level_.levels[level_.level_number]);
    last_spawn_time_ = glutGet(GLUT_ELAPSED_TIME);
}

void Game::RenderGame() {
    RenderLevelDesign();
    if (CheckNextLevel()) {
        if (level_.level_number == 2) DrawText("YOU WIN!", window::WIDTH - 3 * window::WIDTH / 5, window::HEIGHT - window::HEIGHT / 2);
        else {
            for (const auto& obj : objects_) obj->Delete();
            player_.SetPosition({ 7, 9 });
            InitLevel(level_.levels[++level_.level_number]);
        }
    }
    else {
        if (CheckGameOver()) {
            DrawText("GAME OVER!", window::WIDTH - 3 * window::WIDTH / 5, window::HEIGHT - window::HEIGHT / 2);
        }
        else {
            RenderInfoBar();
            PlayerCondition();
            EnemyCondition();

            RenderBullets();
            player_.Render();
            for (const auto& obj : objects_) obj->Render();
            for (const auto& enemy : enemies_) enemy->Render();

            CheckEnemySpawn();
        }
    }
}

void Game::MovePlayer(unsigned char key, int x, int y) {
    switch (key) {
    case 'W':
    case 'w':
        player_.Move(Direction::UP);
        break;
    case 'S':
    case 's':
        player_.Move(Direction::DOWN);
        break;
    case 'A':
    case 'a':
        player_.Move(Direction::LEFT);
        break;
    case 'D':
    case 'd':
        player_.Move(Direction::RIGHT);
        break;
    case 'F':
    case 'f':
        ShootBullet();
        break;
    }
}

void Game::CheckEnemySpawn() {
    int current_time = glutGet(GLUT_ELAPSED_TIME);

    if (enemies_.size() < max_enemies_ && current_time - last_spawn_time_ >= spawn_interval_) {
        SpawnEnemy();
        last_spawn_time_ = current_time;
    }
}

void Game::SpawnEnemy() {
    Point spawn_position = { 15, 0 };
    enemies_.push_back(std::make_unique<Enemy>(spawn_position, objects_, &player_));
}

void Game::EnemyCondition() {
    for (auto it = enemies_.begin(); it != enemies_.end();) {
        bool dead = false;
        for (const auto& bullet : bullets_) {
            if (bullet->GetPosition() == (*it)->GetPosition()) {
                (*it)->Dead();
                dead = true;
                break;
            }
        }
        if (dead) it = enemies_.erase(it);
        else {
            (*it)->Update();
            ++it;
        }
    }
}

void Game::ShootBullet() {
    int current_time = glutGet(GLUT_ELAPSED_TIME);

    if (current_time - last_shoot_time_ >= cooldown_duration_) {
        bullets_.push_back(std::make_unique<Bullet>(player_.GetDirection(), player_.GetPosition(), objects_));
        last_shoot_time_ = current_time;
    }
}

void Game::RenderBullets() {
    for (auto& bullet : bullets_) {
        bullet->Update();
        bullet->Render();
    }

    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
        [](const std::unique_ptr<Bullet>& bullet) {
            return !bullet->IsPositionValid(bullet->GetPosition());
        }), bullets_.end());
}

bool Game::CheckNextLevel() {
    Point disappear_coords = { -1,-1 };
    bool is_all_diappeear = false;
    for (const auto& obj : objects_) {
        if (auto emerald = dynamic_cast<Emerald*>(obj.get())) {
            if (emerald->GetPosition() == disappear_coords) {
                is_all_diappeear = true;
            }
            else {
                is_all_diappeear = false;
                break;
            }
        }
    }
    return is_all_diappeear;
}

bool Game::CheckGameOver() const {
    return lives_ <= 0;
}

void Game::PlayerCondition() {
    for (const auto& obj : objects_) {
        if (obj->GetPosition() == player_.GetPosition()) {
            obj->Update();
            if (auto gold_bag = dynamic_cast<GoldBag*>(obj.get())) {
                if (gold_bag->Move(player_.GetDirection())) score_ += domain::GOLD_SCORE;
            }
            if (auto emerald = dynamic_cast<Emerald*>(obj.get())) {
                score_ += domain::EMERALD_SCORE;
            }
        }
    }
}

void Game::DrawText(const std::string& text, int x, int y) {
    glColor3f(color::WHITE.R, color::WHITE.G, color::WHITE.B);
    glRasterPos2i(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void Game::RenderInfoBar() {
    DrawText("Score: " + std::to_string(score_), window::WIDTH / 5, window::HEIGHT - domain::INFO_BAR_HEIGHT / 2);
    DrawText("Lives: " + std::to_string(lives_), window::WIDTH - 2 * window::WIDTH / 5, window::HEIGHT - domain::INFO_BAR_HEIGHT / 2);
}

void Game::InitLevel(std::string level_name) {
    std::ifstream file(level_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file!" << std::endl;
        return;
    }

    std::string line;
    int y = 0;

    while (std::getline(file, line)) {
        for (size_t x = 0; x < line.size(); ++x) {
            char cell = line[x];
            Point position = { x, y };
            switch (cell) {
            case '.':
                objects_.push_back(std::make_unique<Earth>(position));
                break;
            case 'E':
                objects_.push_back(std::make_unique<Earth>(position));
                objects_.push_back(std::make_unique<Emerald>(position));
                break;
            case 'G':
                objects_.push_back(std::make_unique<Earth>(position));
                objects_.push_back(std::make_unique<GoldBag>(position, objects_, &player_));
                break;
            case 'N':
                enemies_.push_back(std::make_unique<Enemy>(position, objects_, &player_));
                break;
            default:
                break;
            }
        }
        ++y;
    }
    file.close();
}

void Game::RenderLevelDesign() const {
    glColor3f(color::GREEN.R, color::GREEN.G, color::GREEN.B);

    glRecti(design_settings::MARGIN, design_settings::MARGIN, window::WIDTH - design_settings::MARGIN, window::HEIGHT - domain::INFO_BAR_HEIGHT - design_settings::MARGIN);
    glRecti(design_settings::MARGIN, window::HEIGHT - domain::INFO_BAR_HEIGHT, window::WIDTH - design_settings::MARGIN, window::HEIGHT - design_settings::MARGIN);

    glColor3f(color::BLACK.R, color::BLACK.G, color::BLACK.B);
    glRecti(design_settings::MARGIN + design_settings::BOLD, design_settings::MARGIN + design_settings::BOLD, window::WIDTH - design_settings::MARGIN - design_settings::BOLD, window::HEIGHT - domain::INFO_BAR_HEIGHT - design_settings::MARGIN - design_settings::BOLD);
    glRecti(design_settings::MARGIN + design_settings::BOLD, window::HEIGHT - domain::INFO_BAR_HEIGHT + design_settings::BOLD, window::WIDTH - design_settings::MARGIN - design_settings::BOLD, window::HEIGHT - design_settings::MARGIN - design_settings::BOLD);
}
