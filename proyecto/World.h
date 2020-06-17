#pragma once
#ifndef WORLD_H
#define WORLD_H
#include "Serializable.h"

#include <cstring>
#include <SFML/Graphics.hpp>
#include <vector>

class GameObject;

class World : public Serializable
{
public:
    World(sf::RenderWindow* window);
    ~World();

    void render();
    void update(float deltaTime);
    void handleEvent(sf::Event& event);

    void addGameObject(GameObject* gameObject);
    void removeGameObject(GameObject* gameObject);
    void laterRemoveGameObject(GameObject* gameObject);
    const std::vector<GameObject*>& getGameObjects() const;

    void copy(const World& world);

    virtual void to_bin();
    virtual int from_bin(char* data);

    void setIndex(int index);
    int getIndex() const;

    void reset();
    void restart();

private:
    void checkCollisions();

private:
    sf::RenderWindow* window;
    std::vector<GameObject*> gameObjects;

    std::vector<GameObject*> removeObjects;
    int worldIndex;

    bool hasToRestart;
};

#endif