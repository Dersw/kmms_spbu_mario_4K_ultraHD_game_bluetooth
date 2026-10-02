#include "Level.hpp"
#include "Config.hpp"

Level::Level() : currentLevelIndex(1) {}

void Level::loadLevel(int levelIdx) {
    currentLevelIndex = levelIdx;
    staticObjects.clear();
    dynamicObjects.clear();

    if (currentLevelIndex == 1) {
        staticObjects.push_back(std::make_unique<GameObject>(15, 25, 40, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(30,15, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(50, 15, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(60, 20, 40, 10, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(60, 7, 10, 3, Config::TYPE_LINES));
        staticObjects.push_back(std::make_unique<GameObject>(70, 7, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(75, 7, 5, 3, Config::TYPE_LINES));
        staticObjects.push_back(std::make_unique<GameObject>(80, 7, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(80, 7, 10, 3, Config::TYPE_LINES));
        staticObjects.push_back(std::make_unique<GameObject>(100, 25, 20, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(120, 20, 10, 10, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(150, 25, 40, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(210, 20, 10, 10, Config::TYPE_EXIT));

        dynamicObjects.push_back(std::make_unique<Enemy>(25, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(80, 10));
    }
    else if (currentLevelIndex == 2) {
        staticObjects.push_back(std::make_unique<GameObject>(14, 25, 40, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(30, 15, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(50, 15, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(60, 20, 10, 10, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(80, 25, 20, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(120, 20, 10, 10, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(122, 10, 5, 3, Config::TYPE_BOX));
        staticObjects.push_back(std::make_unique<GameObject>(150, 25, 40, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(207, 20, 10, 10, Config::TYPE_EXIT));

        dynamicObjects.push_back(std::make_unique<Enemy>(25, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(80, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(65, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(119, 7));
        dynamicObjects.push_back(std::make_unique<Enemy>(160, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(175, 10));
    }
    else if (currentLevelIndex == 3) {
        staticObjects.push_back(std::make_unique<GameObject>(14, 20, 43, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(80, 20, 16, 5, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(117, 15, 16, 10, Config::TYPE_BRICK));
        staticObjects.push_back(std::make_unique<GameObject>(155, 10, 16, 15, Config::TYPE_EXIT));

        dynamicObjects.push_back(std::make_unique<Enemy>(25, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(50, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(80, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(90, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(120, 10));
        dynamicObjects.push_back(std::make_unique<Enemy>(130, 10));
    }
}

void Level::nextLevel() {
    int nextLvl = currentLevelIndex + 1;
    if (nextLvl > maxLevels) nextLvl = 1;
    loadLevel(nextLvl);
}

void Level::addDynamicObject(std::unique_ptr<GameObject> obj) {
    dynamicObjects.push_back(std::move(obj));
}