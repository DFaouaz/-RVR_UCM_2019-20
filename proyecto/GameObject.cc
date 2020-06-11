#include "GameObject.h"

GameObject::GameObject() : type(NONE)
{
}

GameObject::GameObject(ObjectType type) : type(type)
{
}

GameObject::~GameObject()
{
}

void GameObject::to_bin()
{
    alloc_data(sizeof(type) + sizeof(xPosition) + sizeof(yPosition));

    char *pos = _data;
    memcpy(pos, &type, sizeof(type));

    pos += sizeof(type);
    memcpy(pos, &xPosition, sizeof(xPosition));

    pos += sizeof(xPosition);
    memcpy(pos, &yPosition, sizeof(yPosition));
}

int GameObject::from_bin(char *data)
{
    _size = sizeof(type) + sizeof(xPosition) + sizeof(yPosition);

    char *pos = data;
    memcpy(&type, pos, sizeof(type));

    pos += sizeof(type);
    memcpy(&xPosition, pos, sizeof(xPosition));

    pos += sizeof(xPosition);
    memcpy(&yPosition, pos, sizeof(yPosition));

    return 0;
}

void GameObject::setWorld(World *world)
{
    this->world = world;
}

void GameObject::setPosition(float x, float y)
{
    xPosition = x;
    yPosition = y;
}