#include "PlayerState.h"

PlayerState::PlayerState() : index(-1), xDirection(0), yDirection(0), xAim(0.0f), yAim(0.0f), shooting(false)
{
}

PlayerState::~PlayerState()
{
}

bool PlayerState::handleEvent(sf::Event &event)
{
    PlayerState copy = *this;
    if (event.type == sf::Event::KeyPressed)
    {
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
    else if (event.type == sf::Event::MouseMoved)
    {
        xAim = event.mouseMove.x;
        yAim = event.mouseMove.y;
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        shooting = true;
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        shooting = false;
    }

    return copy == *this;
}