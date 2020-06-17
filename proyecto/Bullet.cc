#include "Bullet.h"
#include <cmath>
#include "Player.h"

Bullet::Bullet() : GameObject(BULLET)
{
    width = 5;
    height = 5;
}

Bullet::Bullet(int index) : GameObject(BULLET), index(index)
{
    width = 5;
    height = 5;
}

Bullet::Bullet(int index, float speed, float xDirection, float yDirection, Player* shooter) : GameObject(BULLET), index(index), speed(speed), xDirection(xDirection), yDirection(yDirection), shooter(shooter)
{
    float magnitude = std::sqrt(xDirection * xDirection + yDirection * yDirection);
    this->xDirection /= magnitude;
    this->yDirection /= magnitude;
    timeSpam = 5.0f;
    width = 5;
    height = 5;
}

Bullet::~Bullet()
{
}

void Bullet::render(sf::RenderWindow *window)
{
    sf::RectangleShape rect({width, height});

    if (world->getIndex() == index)
        rect.setFillColor(sf::Color::Blue);
    else
        rect.setFillColor(sf::Color::Red);

    rect.setPosition(xPosition, yPosition);
    window->draw(rect);
}

void Bullet::update(float deltaTime)
{
    xPosition += xDirection * speed * deltaTime;
    yPosition += yDirection * speed * deltaTime;

    timeSpam -= deltaTime;
    if (timeSpam <= 0)
        world->laterRemoveGameObject(this);
}

void Bullet::onCollisionEnter(GameObject *other)
{
    if (other->type == ObjectType::PLAYER)
    {
        Player *player = (Player *)other;
        if (player->index == index)
            return;
        shooter->kills++;
        world->laterRemoveGameObject(this);
    }
    if (other->type == ObjectType::OBSTACLE)
    {
        world->laterRemoveGameObject(this);
    }
}

void Bullet::to_bin()
{
    GameObject::to_bin();
    int size = GameObject::size();
    char buffer[size];
    memcpy(buffer, GameObject::data(), size);

    alloc_data(size + sizeof(index));

    char *pos = _data;
    memcpy(pos, buffer, size);

    pos += size;
    memcpy(pos, &index, sizeof(index));
}

int Bullet::from_bin(char *data)
{
    GameObject::from_bin(data);

    char *pos = data;
    pos += GameObject::size();
    memcpy(&index, pos, sizeof(index));

    _size += sizeof(index);

    return 0;
}

void Bullet::reset()
{
    world->laterRemoveGameObject(this);
}