#include "Obstacle.h"

Obstacle::Obstacle() : GameObject(OBSTACLE)
{
}

Obstacle::Obstacle(float x, float y, float width, float height) : GameObject(OBSTACLE)
{
    xPosition = x;
    yPosition = y;
    this->width = width;
    this->height = height;
}

Obstacle::~Obstacle()
{
}

void Obstacle::render(sf::RenderWindow *window)
{
    sf::RectangleShape rect({width, height});
    rect.setFillColor(sf::Color::Yellow);
    rect.setPosition(xPosition, yPosition);
    window->draw(rect);
}

void Obstacle::onCollisionEnter(GameObject *other)
{
    if (other->type == ObjectType::PLAYER)
    {
        float xPos, yPos;
        float xSize, ySize;
        other->getPosition(xPos, yPos);
        other->getSize(xSize, ySize);

        float xDiff;
        if (xPosition < xPos)
        {
            xDiff = xPosition + width - xPos;
        }
        else
        {
            xDiff = xPosition - (xPos + xSize);
        }

        float yDiff;
        if (yPosition < yPos)
        {
            yDiff = yPosition + height - yPos;
        }
        else
        {
            yDiff = yPosition - (yPos + ySize);
        }

        xDiff < yDiff ? other->setPosition(xPos + xDiff, yPos) : other->setPosition(xPos, yPos + yDiff);
    }
}

void Obstacle::to_bin()
{
    GameObject::to_bin();
}

int Obstacle::from_bin(char *data)
{
    GameObject::from_bin(data);
    return 0;
}