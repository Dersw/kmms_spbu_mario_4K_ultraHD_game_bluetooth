#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <windows.h>
#include "Entity.hpp"
#include "Level.hpp"
#include "Config.hpp"

class Engine {
private:
    Mario mario;
    Level level;
    char map[Config::MAP_HEIGHT][Config::MAP_WIDTH + 1];
    float cameraX;
    int playerScore;
    bool isRunning;

    void clearMapBuffer();
    void processInput();
    void updatePhysics();
    void checkCollisions();
    void render();
    
    void handleVerticalCollision(GameObject& obj);
    void handleHorizontalEnemyMovement(GameObject& enemy);
    void handleMarioDeath();
    void handleWin();
    
    void putObjectOnMap(const GameObject& obj);
    void putScoreOnMap();
    void setCursorPosition(int x, int y);

public:
    Engine();
    void init();
    void run();
};

#endif