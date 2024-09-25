#pragma once
#include <glut.h>
#include <string>

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return x != other.x || y != other.y;
    }
};

struct PointHash {
    std::size_t operator()(const Point& point) const {
        return std::hash<int>()(point.x) ^ (std::hash<int>()(point.y) << 1);
    }
};

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

namespace window {
    const int HEIGHT = 570;
    const int WIDTH = 690;
    const Point POSITION = { 500,200 };
}
namespace grid {
    const int HEIGHT = 10;
    const int WIDTH = 15;
}
namespace level {
    const int COUNT = 3;
    const std::string FIRST = "levels/1.txt";
    const std::string SECOND = "levels/2.txt";
    const std::string THIRD = "levels/3.txt";
}

namespace color {
    struct Color {
        float R;
        float G;
        float B;
    };

    const Color BLACK = { 0, 0, 0 };
    const Color WHITE = { 1, 1, 1 };

    const Color RED = { 1, 0, 0 };
    const Color GREEN = { 0, 1, 0 };
    const Color BLUE = { 0, 0, 1 };

    const Color YELLOW = { 1, 1, 0 };
    const Color GRAY = { 0.5, 0.5, 0.5 };
    const Color PINK = { 1, 0.75, 0.8 };
    const Color BROWN = { 0.6, 0.4, 0.2 };
}

namespace player {
    const int HEIGHT = 3;
    const int WIDTH = 25;
    const int SPEED = 1;

    const Point START_POS = { 11,16 };
}

namespace design_settings {
    const int BOLD = 2;
    const int MARGIN = 5;
}

namespace domain {
    const double PI = 3.14;
    const int START_SCORE = 0;
    const int START_LIVES = 3;

    const int INFO_BAR_HEIGHT = 105;
    const int OBJECT_SIZE = 10;
    const int EARTH_SIZE = 25;
    const int CELL_SIZE = 46;

    const int BORDER_WIDTH = 2;
    const int FPS = 20;

    const int GOLD_SCORE = 500;
    const int EMERALD_SCORE = 100;
    const int LIVES = 3;
    const int ENEMY_SPEED = 1;
    const int ENEMY_UPDATE = 300;
    const int GOLD_BAG_UPDATE = 100;
    const int SHOOT_COOLDOWN = 2000;

    namespace enemy {
        const int MAX_COUNT = 3;
        const int SPEED = 1;
        const int UPDATE = 300;
        const int SPAWN_INTERVAL = 7000;
    }
}