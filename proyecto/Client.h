#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <SFML/Graphics.hpp>
#include <thread>

#include "MessageClient.h"
#include "MessageServer.h"
#include "Socket.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

class World;

class Client
{
public:
    Client(const std::string& nick, const char * host, const char * port);
    ~Client();

    void init();
    void run();
    void close();

    void recieveMessage();
private:
    // Net stuff
    void login();
    void logout();

    //Logic stuff
    bool processEvents();
    void update(float deltaTime);

private:
    sf::RenderWindow* window;

    // Atributos o propiedades
    sf::Thread* netThread;
    Socket socket;
    std::string nick;
    PlayerState playerState;
    World* world;

    bool terminated;
};

#endif