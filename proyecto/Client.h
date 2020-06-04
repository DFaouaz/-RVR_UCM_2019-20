#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <SDL2/SDL.h>
#include <thread>

#include "MessageClient.h"
#include "MessageServer.h"
#include "Socket.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

class Client
{
public:
    Client(const std::string& nick, const char * host, const char * port);
    ~Client();

    void init();
    void run();
    void close();

private:
    void recieveMessage();

    void login();
    void logout();
    
    bool processEvents();

private:
    // Atributos o propiedades
    SDL_Window* window;

    std::thread recieveThread;
    Socket socket;
    std::string nick;
    PlayerState playerState;

    bool terminated;
};

#endif