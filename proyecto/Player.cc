#include "Player.h"
#include "PlayerState.h"
#include "Bullet.h"

Player::Player(int index) : GameObject(PLAYER), index(index), xDirection(0), yDirection(0), shooting(false), xAim(0.0f), yAim(0.0f), timer(0.0f)
{
    width = 20;
    height = 20;
    xPosition = (index + 1) * 100;
    yPosition = (index + 1) * 100;
}

Player::~Player()
{
}

void Player::render(sf::RenderWindow *window)
{
    sf::RectangleShape rect({width, height});
    rect.setFillColor(sf::Color::White);
    rect.setPosition(xPosition, yPosition);
    window->draw(rect);
}

void Player::update(float deltaTime)
{
    float speed = 200;
    xPosition += xDirection * speed * deltaTime;
    yPosition += yDirection * speed * deltaTime;

    //Logica del disparo
    if (shooting)
    {
        timer -= deltaTime;
        if (timer <= 0)
        {
            shoot();
            timer = 1.0f;
        }
    }
}

void Player::onCollisionEnter(GameObject *other)
{
    if (other->type == ObjectType::BULLET)
    {
        Bullet *bullet = (Bullet *)other;
        if (bullet->index == index)
            return;
    }
}

void Player::shoot()
{
    float xDir = xAim - xPosition;
    float yDir = yAim - yPosition;

    Bullet *bullet = new Bullet(index, 200, xDir, yDir);
    bullet->setPosition(xPosition, yPosition);

    world->addGameObject(bullet);
}

void Player::to_bin()
{
    GameObject::to_bin();
    int size = GameObject::size();
    char buffer[size];
    memcpy(buffer, GameObject::data(), size);

    alloc_data(size + sizeof(index) + sizeof(xDirection) + sizeof(yDirection) + sizeof(shooting) + sizeof(xAim) + sizeof(yAim));

    char *pos = _data;
    memcpy(pos, buffer, size);

    pos += size;
    memcpy(pos, &index, sizeof(index));

    pos += sizeof(index);
    memcpy(pos, &xDirection, sizeof(xDirection));

    pos += sizeof(xDirection);
    memcpy(pos, &yDirection, sizeof(yDirection));

    pos += sizeof(yDirection);
    memcpy(pos, &shooting, sizeof(shooting));

    pos += sizeof(shooting);
    memcpy(pos, &xAim, sizeof(xAim));

    pos += sizeof(xAim);
    memcpy(pos, &yAim, sizeof(yAim));
}

int Player::from_bin(char *data)
{
    GameObject::from_bin(data);

    char *pos = data;
    pos += GameObject::size();
    memcpy(&index, pos, sizeof(index));

    pos += sizeof(index);
    memcpy(&xDirection, pos, sizeof(xDirection));

    pos += sizeof(xDirection);
    memcpy(&yDirection, pos, sizeof(yDirection));

    pos += sizeof(yDirection);
    memcpy(&shooting, pos, sizeof(shooting));

    pos += sizeof(shooting);
    memcpy(&xAim, pos, sizeof(xAim));

    pos += sizeof(xAim);
    memcpy(&yAim, pos, sizeof(yAim));

    _size += sizeof(index) + sizeof(xDirection) + sizeof(yDirection) + sizeof(shooting) + sizeof(xAim) + sizeof(yAim);

    return 0;
}

void Player::processState(const PlayerState &state)
{
    xDirection = state.xDirection;
    yDirection = state.yDirection;

    shooting = state.shooting;

    xAim = state.xAim;
    yAim = state.yAim;

    printf("XDir: %f | YDir: %f | Shooting: %i | XAim: %f | YAim: %f\n", xDirection, yDirection, (int)shooting, xAim, yAim);
}