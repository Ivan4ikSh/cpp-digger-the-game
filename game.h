#include "domain.h"
#include "earth.h"
#include "emerald.h"
#include "enemy.h"
#include "gold_bag.h"
#include "player.h"
#include "bullet.h"

#include <fstream>
#include <iostream>
#include <iostream>
#include <memory>
#include <string>
#include <string>
#include <vector>

class Game {
public:
    Game();
    void RenderGame();
    void MovePlayer(unsigned char key, int x, int y);
private:
    struct Level {
        const std::vector<std::string> levels = { level::FIRST, level::SECOND, level::THIRD };
        int level_number = 0;
    };

    std::vector<std::unique_ptr<GameObject>> objects_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Bullet>> bullets_;
    Player player_{ &lives_ };

    Level level_;
    int lives_ = domain::LIVES;
    int score_ = 0;

    int last_shoot_time_ = 0;
    const int cooldown_duration_ = domain::SHOOT_COOLDOWN;

    int last_spawn_time_ = 0;
    const int max_enemies_ = domain::enemy::MAX_COUNT;
    const int spawn_interval_ = domain::enemy::SPAWN_INTERVAL;

    void InitLevel(std::string level_name);
    
    void PlayerCondition();

    void EnemyCondition();
    void CheckEnemySpawn();
    void SpawnEnemy();
    
    void RenderBullets();
    void ShootBullet();
    
    bool CheckNextLevel();
    bool CheckGameOver() const;

    void DrawText(const std::string& text, int x, int y);
    void RenderInfoBar();
    void RenderLevelDesign() const;
};