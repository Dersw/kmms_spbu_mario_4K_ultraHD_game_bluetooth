#ifndef MAP_HPP
#define MAP_HPP

#include "Config.hpp"


class GameObject; 

class Map {
private:
    char map_data[Config::MAP_HEIGHT][Config::MAP_WIDTH + 1];

    bool isPositionOnMap(int x, int y) const;

public:
    float camera_x;

    Map();

    void clear();
    void putObject(const GameObject& obj);
    void putScore(int score);
    void show() const;
    void setCursorPosition(int x, int y);
};

#endif