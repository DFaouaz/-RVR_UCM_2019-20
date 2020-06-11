#include "World.h"
#include "GameObject.h"
#include <algorithm>
#include "Player.h"
#include "Bullet.h"

World::World(sf::RenderWindow *window) : window(window)
{
}

World::~World()
{
    window = nullptr;
    auto old = gameObjects;
    for (auto gO : old)
    {
        removeGameObject(gO);
    }
}

void World::render()
{
    auto gameObjects = this->gameObjects;

    window->clear();
    for (GameObject *gameObject : gameObjects)
        gameObject->render(window);
    window->display();
}

void World::update(float deltaTime)
{
    auto gameObjects = this->gameObjects;

    for (GameObject *gameObject : gameObjects)
        gameObject->update(deltaTime);
}

void World::handleEvent(sf::Event &event)
{
    auto gameObjects = this->gameObjects;

    for (GameObject *gameObject : gameObjects)
        gameObject->handleEvent(event);
}

void World::addGameObject(GameObject *gameObject)
{
    gameObjects.push_back(gameObject);
}

void World::removeGameObject(GameObject *gameObject)
{
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it == gameObjects.end())
        return;

    gameObjects.erase(it);
    delete gameObject;
}

const std::vector<GameObject *> &World::getGameObjects() const
{
    return gameObjects;
}

void World::copy(const World &world)
{
    // Delete all
    auto old = gameObjects;
    for (auto gO : old)
    {
        removeGameObject(gO);
    }

    for (auto gO : world.getGameObjects())
    {
        GameObject* gameObject = nullptr;
        if (gO->type == ObjectType::PLAYER)
        {
            gameObject = new Player(-1);
            gameObject->xPosition = gO->xPosition;
            gameObject->yPosition = gO->yPosition;
            addGameObject(gameObject);
        }
        else if (gO->type == ObjectType::BULLET)
        {
            gameObject = new Bullet();
            gameObject->xPosition = gO->xPosition;
            gameObject->yPosition = gO->yPosition;
            addGameObject(gameObject);
        }
    }
}

void World::to_bin()
{
    int size = 0;
    for (GameObject *gameObject : gameObjects)
    {
        gameObject->to_bin();
        size += gameObject->size();
    }

    alloc_data(size + sizeof(size_t));

    char *pos = _data;
    size_t count = gameObjects.size();
    memcpy(pos, &count, sizeof(size_t));

    pos += sizeof(size_t);
    for (GameObject *gameObject : gameObjects)
    {
        memcpy(pos, gameObject->data(), gameObject->size());
        pos += gameObject->size();
    }
}

int World::from_bin(char *data)
{
    // Remove all
    auto old = gameObjects;
    for (auto gO : old)
    {
        removeGameObject(gO);
    }

    char *pos = data;
    size_t count = 0;
    memcpy(&count, pos, sizeof(size_t));

    printf("%i\n", (int)count);

    pos += sizeof(size_t);
    while (count > 0)
    {
        ObjectType auxType;
        memcpy(&auxType, pos, sizeof(auxType));

        GameObject *gameObject = nullptr;
        if (auxType == ObjectType::PLAYER)
        {
            gameObject = new Player(-1);
            gameObject->from_bin(pos);
            pos += gameObject->size();
        }
        else if (auxType == ObjectType::BULLET)
        {
            gameObject = new Bullet();
            gameObject->from_bin(pos);
            pos += gameObject->size();
        }
        if (gameObject != nullptr)
            addGameObject(gameObject);

        count--;
    }

    return 0;
}