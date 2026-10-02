#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>
#include <memory>
#include "Entity.hpp"

class Level {
private:
    std::vector<std::unique_ptr<GameObject>> staticObjects;
    std::vector<std::unique_ptr<GameObject>> dynamicObjects;
    int currentLevelIndex;
    const int maxLevels = 3;

public:
    Level();
    
    void loadLevel(int levelIdx);
    void nextLevel();
    void addDynamicObject(std::unique_ptr<GameObject> obj);

    std::vector<std::unique_ptr<GameObject>>& getStaticObjects() { return staticObjects; }
    std::vector<std::unique_ptr<GameObject>>& getDynamicObjects() { return dynamicObjects; }
    int getCurrentLevelIndex() const { return currentLevelIndex; }
};

#endif