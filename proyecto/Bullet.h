#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Player;

class Bullet : public GameObject
{
public:
    Bullet();
    Bullet(int index);
    Bullet(int index, float speed, float xDirection, float yDirection, Player* shooter);
    virtual ~Bullet();

    virtual void render(sf::RenderWindow* window);
    virtual void update(float deltaTime);
    virtual void handleEvent(sf::Event &event) {};
    virtual void onCollisionEnter(GameObject* other);

    virtual void to_bin();
    virtual int from_bin(char *data);

    virtual void reset();

public:
    int index;
    Player* shooter;
    
private:
    float timeSpam;
    float speed;
    float xDirection;
    float yDirection;
};

#endif