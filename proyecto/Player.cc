#include "Player.h"
#include "PlayerState.h"
#include "Bullet.h"

Player::Player(int index) : GameObject(PLAYER), index(index), xDirection(0), yDirection(0), shooting(false), xAim(0.0f), yAim(0.0f), timer(0.0f)
{
    kills = 0;
    width = 20;
    height = 20;
    hasToRespawn = false;
    respawn();
}

Player::~Player()
{
}

void Player::render(sf::RenderWindow *window)
{
    sf::RectangleShape rect({width, height});
    if (world->getIndex() == index)
        rect.setFillColor(sf::Color::Green);
    else
        rect.setFillColor(sf::Color(255, 164, 32, 255));

    rect.setPosition(xPosition, yPosition);
    window->draw(rect);

    sf::Font font;
    if (font.loadFromFile("./assets/arial.ttf"))
    {
        sf::Text killCounter(std::to_string(kills), font, 20);
        killCounter.setOrigin(0.5f, 0.5f);
        killCounter.setFillColor(sf::Color::White);
        killCounter.setPosition(xPosition + width / 4.0f, yPosition - height / 10.0f);
        killCounter.setOutlineColor(sf::Color::Black);
        killCounter.setOutlineThickness(1.0f);
        killCounter.setStyle(sf::Text::Bold);
        window->draw(killCounter);
    }
}

void Player::update(float deltaTime)
{
    if(hasToRespawn)
    {
        hasToRespawn = false;
        respawn();
    }

    float speed = 200;
    xPosition += xDirection * speed * deltaTime;
    yPosition += yDirection * speed * deltaTime;

    if (xPosition < 0)
        xPosition = 0;
    else if (xPosition + width > SCREEN_WIDTH)
        xPosition = SCREEN_WIDTH - width;

    if (yPosition < 0)
        yPosition = 0;
    else if (yPosition + height > SCREEN_HEIGHT)
        yPosition = SCREEN_HEIGHT - height;

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

    if(kills >= 30)
        world->restart();
}

void Player::onCollisionEnter(GameObject *other)
{
    if (other->type == ObjectType::BULLET)
    {
        Bullet *bullet = (Bullet *)other;
        if (bullet->index == index)
            return;
        hasToRespawn = true;
    }
}

void Player::shoot()
{
    float xDir = xAim - xPosition;
    float yDir = yAim - yPosition;

    Bullet *bullet = new Bullet(index, 200, xDir, yDir, this);
    bullet->setPosition(xPosition, yPosition);

    world->addGameObject(bullet);
}

void Player::respawn()
{
    float x = random(0.0f, float(SCREEN_WIDTH) - width);
    float y = random(0.0f, float(SCREEN_HEIGHT) - height);

    setPosition(x, y);
}

void Player::to_bin()
{
    GameObject::to_bin();
    int size = GameObject::size();
    char buffer[size];
    memcpy(buffer, GameObject::data(), size);

    alloc_data(size + sizeof(index) + sizeof(kills) + sizeof(xDirection) + sizeof(yDirection) + sizeof(shooting) + sizeof(xAim) + sizeof(yAim));

    char *pos = _data;
    memcpy(pos, buffer, size);

    pos += size;
    memcpy(pos, &index, sizeof(index));

    pos += sizeof(index);
    memcpy(pos, &kills, sizeof(kills));

    pos += sizeof(kills);
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
    memcpy(&kills, pos, sizeof(kills));

    pos += sizeof(kills);
    memcpy(&xDirection, pos, sizeof(xDirection));

    pos += sizeof(xDirection);
    memcpy(&yDirection, pos, sizeof(yDirection));

    pos += sizeof(yDirection);
    memcpy(&shooting, pos, sizeof(shooting));

    pos += sizeof(shooting);
    memcpy(&xAim, pos, sizeof(xAim));

    pos += sizeof(xAim);
    memcpy(&yAim, pos, sizeof(yAim));

    _size += sizeof(index) + sizeof(kills) + sizeof(xDirection) + sizeof(yDirection) + sizeof(shooting) + sizeof(xAim) + sizeof(yAim);

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

void Player::reset()
{
    respawn();
    kills = 0;
}

float Player::random(float min, float max)
{
    return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}