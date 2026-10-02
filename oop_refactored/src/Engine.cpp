#include "Engine.hpp"
#include <cstdio>
#include <cmath>
#include <conio.h>

Engine::Engine() : cameraX(0.0f), playerScore(0), isRunning(true) {}

void Engine::init() {
    printf("Управление: A/D - вперед/назад, Пробел - прыжок, ESC - выход\n");
    printf("Нажмите любую клавишу...");
    _getch();
    
    system(Config::COLOR_DEFAULT);
    level.loadLevel(1);
}

void Engine::run() {
    while (isRunning) {
        clearMapBuffer();
        processInput();
        updatePhysics();
        checkCollisions();
        render();
        Sleep(Config::FRAME_DELAY_MS);
    }
}

void Engine::clearMapBuffer() {
    for (int i = 0; i < Config::MAP_WIDTH; i++) map[0][i] = ' ';
    map[0][Config::MAP_WIDTH] = '\0';
    for (int i = 0; i < Config::MAP_WIDTH; i++) map[1][i] = ' ';

    for (int j = 1; j < Config::MAP_HEIGHT; j++) {
        sprintf_s(map[j], "%s", map[0]);
    }
}

void Engine::processInput() {
    if (GetKeyState(VK_ESCAPE) < 0) isRunning = false;
    if (GetKeyState(VK_SPACE) < 0)  mario.jump();

    float moveSpeed = 1.5f;  
    float dx = 0;
    if (GetKeyState('A') < 0) dx = -moveSpeed;
    if (GetKeyState('D') < 0) dx = moveSpeed;

    if (dx != 0) {
        float oldX = mario.getX();
        mario.setX(oldX + dx);

        for (const auto& brick : level.getStaticObjects()) {
            if (mario.checkCollision(*brick)) {
                mario.setX(oldX);
                break;
            }
        }
    }

    cameraX = mario.getX() - (Config::MAP_WIDTH / 4.0f);
    

    if (cameraX < 0) cameraX = 0;
}

void Engine::updatePhysics() {
    mario.updateVerticalPhysics();

    for (auto& brick : level.getStaticObjects()) {
        if (mario.checkCollision(*brick)) {
            if (mario.getVSpeed() > 0) {
                mario.setIsFlying(false);
            }
            if (brick->getType() == Config::TYPE_BOX && mario.getVSpeed() < 0) {
                brick->setType(Config::TYPE_LINES);
                level.addDynamicObject(std::make_unique<Coin>(brick->getX() + 2.0f, brick->getY() - 2.5f));
            }
            mario.setY(mario.getY() - mario.getVSpeed());
            mario.setVSpeed(0);
            if (brick->getType() == Config::TYPE_EXIT) {
                handleWin();
                return;
            }
            break;
        }
    }

    if (mario.getY() > Config::MAP_HEIGHT || mario.getY() < 0) {
        handleMarioDeath();
        return;
    }

    for (size_t i = 0; i < level.getDynamicObjects().size(); ++i) {
        auto& obj = level.getDynamicObjects()[i];
        handleVerticalCollision(*obj);
        
        if (obj->getType() == Config::TYPE_ENEMY) {
            handleHorizontalEnemyMovement(*obj);
        }
        else if (obj->getType() == Config::TYPE_COIN) {
            obj->setX(obj->getX() + obj->getHSpeed());
            for (const auto& brick : level.getStaticObjects()) {
                if (obj->checkCollision(*brick)) {
                    obj->setX(obj->getX() - obj->getHSpeed());
                    obj->reverseHorizontalDirection();
                    break;
                }
            }
        }
    }
}

void Engine::handleVerticalCollision(GameObject& obj) {
    obj.updateVerticalPhysics();

    for (auto& brick : level.getStaticObjects()) {
        if (obj.checkCollision(*brick)) {
            if (obj.getVSpeed() > 0) {
                obj.setIsFlying(false);
            }

            if (brick->getType() == Config::TYPE_BOX && obj.getVSpeed() < 0 && &obj == &mario) {
                brick->setType(Config::TYPE_LINES);
                level.addDynamicObject(std::make_unique<Coin>(brick->getX() + 2.0f, brick->getY() - 2.5f));
            }

            obj.setY(obj.getY() - obj.getVSpeed());
            obj.setVSpeed(0);

            if (brick->getType() == Config::TYPE_EXIT && &obj == &mario) {
                handleWin();
                return;
            }
            break;
        }
    }
}

void Engine::handleHorizontalEnemyMovement(GameObject& enemy) {
    enemy.setX(enemy.getX() + enemy.getHSpeed());

    for (const auto& brick : level.getStaticObjects()) {
        if (enemy.checkCollision(*brick)) {
            enemy.setX(enemy.getX() - enemy.getHSpeed());
            enemy.reverseHorizontalDirection();
            return;
        }
    }

    GameObject virtualTmp = enemy;
    virtualTmp.updateVerticalPhysics();
    bool willFly = true;
    for (const auto& brick : level.getStaticObjects()) {
        if (virtualTmp.checkCollision(*brick)) {
            willFly = false;
            break;
        }
    }
    if (willFly) {
        enemy.setX(enemy.getX() - enemy.getHSpeed());
        enemy.reverseHorizontalDirection();
    }
}

void Engine::checkCollisions() {
    auto& dynamicObjects = level.getDynamicObjects();
    for (size_t i = 0; i < dynamicObjects.size(); ++i) {
        if (mario.checkCollision(*dynamicObjects[i])) {
            
            if (dynamicObjects[i]->getType() == Config::TYPE_ENEMY) {
                float marioCenterY = mario.getY() + (mario.getHeight() / 2.0f);
                float enemyCenterY = dynamicObjects[i]->getY() + (dynamicObjects[i]->getHeight() / 2.0f);
                
               
                if (mario.getVSpeed() > 0 && mario.getY() + mario.getHeight() < enemyCenterY + 1.0f) {
                    playerScore += Config::SCORE_FOR_KILL;
                    dynamicObjects.erase(dynamicObjects.begin() + i);
                    i--;
                    mario.setVSpeed(-1.5f);
                    mario.setIsFlying(true);
                } else {
                 
                    handleMarioDeath();
                    return;
                }
            } 
            else if (dynamicObjects[i]->getType() == Config::TYPE_COIN) {
                playerScore += Config::SCORE_FOR_COIN;
                dynamicObjects.erase(dynamicObjects.begin() + i);
                i--;
            }
        }
    }
}

void Engine::handleMarioDeath() {
    system(Config::COLOR_LOSE);
    Sleep(Config::RESTART_DELAY_MS);
    playerScore = 0;
    mario.resetPosition();
    level.loadLevel(level.getCurrentLevelIndex());
	
	system("cls");
    system(Config::COLOR_DEFAULT);
}

void Engine::handleWin() {
    system(Config::COLOR_WIN);
    Sleep(Config::RESTART_DELAY_MS);
    mario.resetPosition();
    level.nextLevel();
    system(Config::COLOR_DEFAULT);
}

void Engine::putObjectOnMap(const GameObject& obj) {
    int ix = static_cast<int>(std::round(obj.getX() - cameraX));
    int iy = static_cast<int>(std::round(obj.getY()));
    int iW = static_cast<int>(std::round(obj.getWidth()));
    int iH = static_cast<int>(std::round(obj.getHeight()));

    for (int i = ix; i < (ix + iW); ++i) {
        for (int j = iy; j < (iy + iH); ++j) {
            if (i >= 0 && i < Config::MAP_WIDTH && j >= 0 && j < Config::MAP_HEIGHT && j > 1) {
                map[j][i] = obj.getType();
            }
        }
    }
}

void Engine::putScoreOnMap() {
    for (int i = Config::SCORE_X_OFFSET; i < Config::SCORE_AREA_WIDTH; ++i) {
        map[1][i] = ' ';
    }
    char scoreStr[30];
    sprintf_s(scoreStr, "player_score: %d | Level: %d", playerScore, level.getCurrentLevelIndex());
    size_t len = strlen(scoreStr);
    for (size_t i = 0; i < len; ++i) {
        map[1][i + Config::SCORE_X_OFFSET] = scoreStr[i];
    }
}

void Engine::setCursorPosition(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Engine::render() {
    for (const auto& brick : level.getStaticObjects())  putObjectOnMap(*brick);
    for (const auto& dyn : level.getDynamicObjects())     putObjectOnMap(*dyn);
    putObjectOnMap(mario);
    putScoreOnMap();

    setCursorPosition(0, 0);
    for (int j = 0; j < Config::MAP_HEIGHT; j++) {
        printf("%s\n", map[j]);
    }
}
