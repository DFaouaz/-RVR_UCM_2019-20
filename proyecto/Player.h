#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class PlayerState;

class Player : public GameObject
{
public:
    Player(int index);
    virtual ~Player();

    virtual void render(sf::RenderWindow *window);
    virtual void update(float deltaTime);
    virtual void handleEvent(sf::Event &event) {};

    virtual void to_bin();
    virtual int from_bin(char *data);

    void processState(const PlayerState& state);

private:
    void shoot();

private:
    int index;
    float xDirection;
    float yDirection;
    bool shooting;
    float xAim;
    float yAim;

    float timer;
};

#endif