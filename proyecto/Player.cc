#include "Player.h"
#include "PlayerState.h"

Player::Player(int index) : GameObject(ObjectType::PLAYER), index(index), xDirection(0), yDirection(0)
{
    xPosition = (index + 1) * 100;
    yPosition = (index + 1) * 100;
}

Player::~Player()
{
}

void Player::render(sf::RenderWindow *window)
{
    sf::RectangleShape rect({20, 20});
    rect.setFillColor(sf::Color::White);
    rect.setPosition(xPosition, yPosition);
    window->draw(rect);
}

void Player::update(float deltaTime)
{
    float speed = 200;
    xPosition += xDirection * speed * deltaTime;
    yPosition += yDirection * speed * deltaTime;
}

void Player::handleEvent(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        printf("KEY PRESSED\n");
        if (event.key.code == sf::Keyboard::A)
            xDirection = -1;
        if (event.key.code == sf::Keyboard::D)
            xDirection = 1;
        if (event.key.code == sf::Keyboard::W)
            yDirection = -1;
        if (event.key.code == sf::Keyboard::S)
            yDirection = 1;
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::A)
            xDirection = 0;
        if (event.key.code == sf::Keyboard::D)
            xDirection = 0;
        if (event.key.code == sf::Keyboard::W)
            yDirection = 0;
        if (event.key.code == sf::Keyboard::S)
            yDirection = 0;
    }
}

void Player::to_bin()
{
    GameObject::to_bin();
    int size = GameObject::size();
    char buffer[size];
    memcpy(buffer, GameObject::data(), size);

    alloc_data(size + sizeof(index) + sizeof(xDirection) + sizeof(yDirection));

    char* pos = _data;
    memcpy(pos, buffer, size);

    pos += size;
    memcpy(pos, &index, sizeof(index));

    pos += sizeof(index);
    memcpy(pos, &xDirection, sizeof(xDirection));

    pos += sizeof(xDirection);
    memcpy(pos, &yDirection, sizeof(yDirection));
}

int Player::from_bin(char *data)
{
    GameObject::from_bin(data);

    char* pos = data;
    pos += GameObject::size();
    memcpy(&index, pos, sizeof(index));

    pos += sizeof(index);
    memcpy(&xDirection, pos, sizeof(xDirection));

    pos += sizeof(xDirection);
    memcpy(&yDirection, pos, sizeof(yDirection));

    _size += sizeof(index) + sizeof(xDirection) + sizeof(yDirection);

    return 0;
}

void Player::processState(const PlayerState &state)
{
    xDirection = state.xDirection;
    yDirection = state.yDirection;

    //TODO completar
}