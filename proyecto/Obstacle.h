#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
    Obstacle();
    Obstacle(float x, float y, float width, float height);
    virtual ~Obstacle();

    virtual void render(sf::RenderWindow *window);
    virtual void update(float deltaTime) {};
    virtual void handleEvent(sf::Event &event) {};
    virtual void onCollisionEnter(GameObject *other);

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif