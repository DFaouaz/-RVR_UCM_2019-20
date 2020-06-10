#include "World.h"
#include "GameObject.h"
#include <algorithm>
#include <Player.h>

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
    window->clear();
    for (GameObject *gameObject : gameObjects)
        gameObject->render(window);
    window->display();
}

void World::update(float deltaTime)
{
    for (GameObject *gameObject : gameObjects)
        gameObject->update(deltaTime);
}

void World::handleEvent(sf::Event &event)
{
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
        if(gO->type == ObjectType::PLAYER)
        {
            GameObject* player = new Player(-1);
            player->xPosition = gO->xPosition;
            player->yPosition = gO->yPosition;
            addGameObject(player);
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

    char* pos = data;
    size_t count = 0;
    memcpy(&count, pos, sizeof(size_t));

    printf("%i\n", (int) count);

    pos += sizeof(size_t);
    while(count > 0)
    {
        ObjectType auxType;
        memcpy(&auxType, pos, sizeof(auxType));

        if(auxType == ObjectType::PLAYER)
        {
            GameObject* player = new Player(-1);
            player->from_bin(pos);
            pos += player->size();
            addGameObject(player);
        }

        count--;
    }
    
    return 0;
}