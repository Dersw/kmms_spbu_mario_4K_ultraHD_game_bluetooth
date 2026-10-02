#include "Entity.hpp"
#include "Config.hpp"
#include <cmath>

GameObject::GameObject(float x, float y, float w, float h, char t)
    : x(x), y(y), width(w), height(h), type(t), vSpeed(0.0f), hSpeed(0.0f), isFlying(true) {}

bool GameObject::checkCollision(const GameObject& other) const {
    return (x < other.x + other.width &&
            x + width > other.x &&
            y < other.y + other.height &&
            y + height > other.y);
}

void GameObject::updateVerticalPhysics() {
    vSpeed += Config::GRAVITY;
    y += vSpeed;
    isFlying = true;
}

Mario::Mario() : GameObject(Config::MARIO_START_X, Config::MARIO_START_Y, 4.0f, 4.0f, Config::TYPE_MARIO) {}

void Mario::jump() {
    if (!isFlying) {
        vSpeed = Config::JUMP_FORCE;
        isFlying = true;
    }
}

void Mario::resetPosition() {
    x = Config::MARIO_START_X;
    y = Config::MARIO_START_Y;
    vSpeed = 0;
    isFlying = true;
}

Enemy::Enemy(float x, float y) : GameObject(x, y, 4.0f, 3.0f, Config::TYPE_ENEMY) {
    hSpeed = Config::ENEMY_SPEED;
}

Coin::Coin(float x, float y) : GameObject(x, y, 2.0f, 2.0f, Config::TYPE_COIN) {
    hSpeed = Config::COIN_SPEED;
}