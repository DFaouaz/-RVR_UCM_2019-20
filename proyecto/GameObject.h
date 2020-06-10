#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Serializable.h"
#include <SFML/Graphics.hpp>
#include "World.h"

enum ObjectType
{
    PLAYER,
    BULLET
};

class GameObject : public Serializable
{
    friend class World;
public:
    GameObject();
    GameObject(ObjectType type);
    virtual ~GameObject();

public:
    virtual void render(sf::RenderWindow *window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void handleEvent(sf::Event &event) = 0;

    virtual void to_bin();
    virtual int from_bin(char *data);

public:
    ObjectType type;

protected:
    float xPosition;
    float yPosition;
};

#endif