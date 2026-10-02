#include "Map.hpp"
#include "Entity.hpp"
#include <cstdio>
#include <cmath>
#include <cstring>
#include <windows.h>

Map::Map() {
    camera_x = 0.0f;
    clear();
}

void Map::clear() {
    for (int i = 0; i < Config::MAP_WIDTH; i++) {
        map_data[0][i] = ' ';
    }
    map_data[0][Config::MAP_WIDTH] = '\0';

    for (int i = 0; i < Config::MAP_WIDTH; i++) {
        map_data[1][i] = ' ';
    }

    for (int j = 1; j < Config::MAP_HEIGHT; j++) {
        sprintf(map_data[j], "%s", map_data[0]);
    }
}

bool Map::isPositionOnMap(int x, int y) const {
    return ((x >= 0) && (x < Config::MAP_WIDTH) &&
            (y >= 0) && (y < Config::MAP_HEIGHT));
}


void Map::putObject(const GameObject& obj) {
    const int ix = static_cast<int>(std::round(obj.getX() - camera_x));
    const int iy = static_cast<int>(std::round(obj.getY()));
    const int iWidth = static_cast<int>(std::round(obj.getWidth()));
    const int iHeight = static_cast<int>(std::round(obj.getHeight()));

    for (int i = ix; i < (ix + iWidth); i++) {
        for (int j = iy; j < (iy + iHeight); j++) {
            if (isPositionOnMap(i, j) && j > 1) {
                map_data[j][i] = obj.getType();
            }
        }
    }
}

void Map::putScore(int score) {
    for (int i = Config::SCORE_X_OFFSET; i < Config::SCORE_AREA_WIDTH; i++) {
        map_data[1][i] = ' ';
    }

    char c[30];
    sprintf(c, "player_score: %d", score);
    const int len = static_cast<int>(strlen(c));

    for (int i = 0; i < len; i++) {
        map_data[1][i + Config::SCORE_X_OFFSET] = c[i];
    }
}

void Map::setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = static_cast<SHORT>(x);
    coord.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Map::show() const {
    for (int j = 0; j < Config::MAP_HEIGHT; j++) {
        printf("%s\n", map_data[j]);
    }
}