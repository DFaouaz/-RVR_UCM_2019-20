#include "Client.h"
#include "Message.h"

Client::Client(const std::string& nick, const char * host, const char * port) : window(nullptr), nick(nick), socket(host, port)
{

}

Client::~Client()
{

}

// Public methods

void Client::init()
{
    // SDL Init
    /*if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    //Create window
    window = SDL_CreateWindow("Shooter RVR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }*/

    // Envia señal de login
    login();

}

void Client::run()
{
    while (true)
        ;
}

void Client::close()
{
    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}

void Client::login()
{
    PlayerState playerState;

    Message message(nick, playerState);
    message.type = Message::LOGIN;

    socket.send(message, socket);
}

void Client::logout()
{
    PlayerState playerState;

    Message message(nick, playerState);
    message.type = Message::LOGOUT;

    socket.send(message, socket);
}