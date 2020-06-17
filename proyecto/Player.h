#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

class PlayerState;

class Player : public GameObject
{
public:
    Player(int index);
    virtual ~Player();

    virtual void render(sf::RenderWindow *window);
    virtual void update(float deltaTime);
    virtual void handleEvent(sf::Event &event){};
    virtual void onCollisionEnter(GameObject *other);

    virtual void to_bin();
    virtual int from_bin(char *data);

    void processState(const PlayerState &state);

    virtual void reset();

private:
    void shoot();
    void respawn();
    float random(float min, float max);

public:
    int index;
    int kills;

private:
    bool hasToRespawn;
    float xDirection;
    float yDirection;
    bool shooting;
    float xAim;
    float yAim;

    float timer;
};

#endif