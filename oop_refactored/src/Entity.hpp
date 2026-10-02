#ifndef ENTITY_HPP
#define ENTITY_HPP

class GameObject {
protected:
    float x, y;
    float width, height;
    float vSpeed, hSpeed;
    char type;
    bool isFlying;

public:
    GameObject(float x, float y, float w, float h, char t);
    virtual ~GameObject() = default;

    bool checkCollision(const GameObject& other) const;
    void updateVerticalPhysics();

    float getX() const { return x; }
    void setX(float val) { x = val; }
    float getY() const { return y; }
    void setY(float val) { y = val; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    char getType() const { return type; }
    void setType(char t) { type = t; }
    float getVSpeed() const { return vSpeed; }
    void setVSpeed(float val) { vSpeed = val; }
    float getHSpeed() const { return hSpeed; }
    bool getIsFlying() const { return isFlying; }
    void setIsFlying(bool val) { isFlying = val; }

    void reverseHorizontalDirection() { hSpeed = -hSpeed; }
};

class Mario : public GameObject {
public:
    Mario();
    void jump();
    void resetPosition();
};

class Enemy : public GameObject {
public:
    Enemy(float x, float y);
};

class Coin : public GameObject {
public:
    Coin(float x, float y);
};

#endif