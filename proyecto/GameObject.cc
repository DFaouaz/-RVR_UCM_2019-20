#include "GameObject.h"

GameObject::GameObject() : type(NONE), width(1.0f), height(1.0f)
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
    alloc_data(sizeof(type) + sizeof(xPosition) + sizeof(yPosition) + sizeof(width) + sizeof(height));

    char *pos = _data;
    memcpy(pos, &type, sizeof(type));

    pos += sizeof(type);
    memcpy(pos, &xPosition, sizeof(xPosition));

    pos += sizeof(xPosition);
    memcpy(pos, &yPosition, sizeof(yPosition));

    pos += sizeof(yPosition);
    memcpy(pos, &width, sizeof(width));

    pos += sizeof(width);
    memcpy(pos, &height, sizeof(height));
}

int GameObject::from_bin(char *data)
{
    _size = sizeof(type) + sizeof(xPosition) + sizeof(yPosition) + sizeof(width) + sizeof(height);

    char *pos = data;
    memcpy(&type, pos, sizeof(type));

    pos += sizeof(type);
    memcpy(&xPosition, pos, sizeof(xPosition));

    pos += sizeof(xPosition);
    memcpy(&yPosition, pos, sizeof(yPosition));

    pos += sizeof(yPosition);
    memcpy(&width, pos, sizeof(width));

    pos += sizeof(width);
    memcpy(&height, pos, sizeof(height));

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

void GameObject::setSize(float x, float y)
{
    width = x;
    height = y;
}

void GameObject::getPosition(float& x, float& y)
{
    x = xPosition;
    y = yPosition;
}

void GameObject::getSize(float& x, float& y)
{
    x = width;
    y = height;
}

bool GameObject::checkCollision(GameObject* other)
{
    // Calculos
    return  xPosition < other->xPosition + other->width &&
            xPosition + width > other->xPosition &&
            yPosition < other->yPosition + other->height &&
            yPosition + height > other->yPosition;
}