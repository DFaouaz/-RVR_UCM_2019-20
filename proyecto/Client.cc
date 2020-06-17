#include "Client.h"
#include <X11/Xlib.h>

Client::Client(const std::string &nick, const char *host, const char *port) : window(nullptr), nick(nick), socket(host, port), terminated(false), world(nullptr)
{
}

Client::~Client()
{
}

// Public methods

void Client::init()
{
    XInitThreads();
    window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16), "SHUBE");
    window->setActive(false);
    world = new World(window);

    netThread = new sf::Thread(&Client::recieveMessage, this);
    netThread->launch();

    // Login
    login();
}

void Client::run()
{
    while (!terminated && window->isOpen())
    {
        if (!processEvents())
            break;
    }
}

void Client::close()
{
    // Logout
    logout();

    terminated = true;
    netThread->wait();

    delete world;
    delete netThread;
    delete window;
}

void Client::recieveMessage()
{
    while (!terminated)
    {
        MessageServer message;
        socket.recv(message);

        if (message.type == MessageServer::LOGIN)
        {
            if (message.index == -1)
            {
                printf("Partida llena\n");
                terminated = true;
                break;
            }
            playerState.index = message.index;
            world->setIndex(message.index);
        }

        if (message.type == MessageServer::LOGOUT)
        {
            printf("Conexion cerrada\n");
            terminated = true;
            break;
        }

        world->copy(message.world);
        world->render();
    }
}

void Client::login()
{
    MessageClient message(nick, playerState);
    message.type = MessageClient::LOGIN;
    message.playerState = playerState;

    socket.send(message, socket);
}

void Client::logout()
{
    MessageClient message(nick, playerState);
    message.type = MessageClient::LOGOUT;
    message.playerState = playerState;

    socket.send(message, socket);
}

bool Client::processEvents()
{
    sf::Event event;
    bool processed = false;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            return false;
        processed = !playerState.handleEvent(event) || processed;
    }

    if (processed)
    {
        MessageClient messageClient;
        messageClient.type = MessageClient::MESSAGE;
        messageClient.nick = nick;
        messageClient.playerState = playerState;

        socket.send(messageClient, socket);
    }

    return true;
}
