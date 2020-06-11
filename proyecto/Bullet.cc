#include "Bullet.h"
#include <cmath>

Bullet::Bullet() : GameObject(BULLET)
{
}

Bullet::Bullet(float speed, float xDirection, float yDirection) : GameObject(BULLET), speed(speed), xDirection(xDirection), yDirection(yDirection)
{
    float magnitude = std::sqrt(xDirection * xDirection + yDirection * yDirection);
    xDirection /= magnitude;
    yDirection /= magnitude;
}

Bullet::~Bullet()
{
}

void Bullet::render(sf::RenderWindow *window)
{
    sf::RectangleShape rect({5, 5});
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(xPosition, yPosition);
    window->draw(rect);
}

void Bullet::update(float deltaTime)
{
    xPosition += xDirection * speed * deltaTime;
    yPosition += yDirection * speed * deltaTime;
}

void Bullet::to_bin()
{
    GameObject::to_bin();
}

int Bullet::from_bin(char *data)
{
    GameObject::from_bin(data);
    return 0;
}