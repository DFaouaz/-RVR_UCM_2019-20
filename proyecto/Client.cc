#include "Client.h"

Client::Client(const std::string &nick, const char *host, const char *port) : window(nullptr), nick(nick), socket(host, port), terminated(false)
{
}

Client::~Client()
{
}

// Public methods

void Client::init()
{
    // SDL Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    //Create window
    window = SDL_CreateWindow("Shube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    recieveThread = std::thread(&Client::recieveMessage, std::ref(*this));

    // Envia señal de login
    login();
}

void Client::run()
{
    while (true)
    {
        // Recibir mensaje TODO: hacer un hilo para esto
        //recieveMessage();
        // Renderizar

        // Porcesar eventos
        playerState = PlayerState();
        if(!processEvents()) break;

        // Enviar mensaje del estado
        /*MessageClient message;
        message.type = MessageClient::MESSAGE;
        message.playerState = playerState;
        socket.send(message, socket);*/
    }
}

void Client::close()
{
    terminated = true;

    // Enviar logout
    logout();
    recieveThread.join();

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

}

void Client::recieveMessage()
{
    while (!terminated)
    {

        MessageServer message;
        socket.recv(message);

        if(message.type == MessageServer::LOGOUT)
        {
            printf("LOGOUT\n");
            break;
        }

        // Imprimimos el world State
        printf("NPlayers : %d\n", message.worldState.nPlayers);
        for (int i = 0; i < 4; i++)
        {
            printf("x: %f | y: %f\n", message.worldState.pPositions[i].x, message.worldState.pPositions[i].y);
        }
    }
}

void Client::login()
{
    PlayerState playerState;

    MessageClient message(nick, playerState);
    message.type = MessageClient::LOGIN;

    socket.send(message, socket);
}

void Client::logout()
{
    PlayerState playerState;

    MessageClient message(nick, playerState);
    message.type = MessageClient::LOGOUT;

    socket.send(message, socket);
}

bool Client::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;

        /*if (event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_w){
                playerState.yDirection = 1;
            }
            else if(event.key.keysym.sym == SDLK_s){
                playerState.yDirection = -1;
            }
            else if(event.key.keysym.sym == SDLK_a){

                playerState.xDirection = -1;
            }
            else if(event.key.keysym.sym == SDLK_d){
                playerState.xDirection = 1;
            }
        }*/
    }
    return true;
}