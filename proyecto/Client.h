#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <SDL2/SDL.h>

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

    void login();
    void logout();
    
    void processEvents() const;
    void processEvent(const SDL_Event& event) const;

private:
    // Atributos o propiedades
    SDL_Window* window;

    Socket socket;
    std::string nick;
};

#endif