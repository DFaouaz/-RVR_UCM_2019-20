#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Serializable.h"
#include <SFML/Graphics.hpp>
#include "World.h"

enum ObjectType
{
    NONE,
    PLAYER,
    BULLET,
    OBSTACLE
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
    virtual void onCollisionEnter(GameObject* other) {};

    virtual void to_bin();
    virtual int from_bin(char *data);

    void setWorld(World* world);
    void setPosition(float x, float y);
    void setSize(float x, float y);

    void getPosition(float& x, float& y);
    void getSize(float& x, float& y);

    bool checkCollision(GameObject* gameObject);

public:
    ObjectType type;

protected:
    World* world;
    float xPosition;
    float yPosition;
    float width;
    float height;
};

#endif