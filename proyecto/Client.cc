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
    while (window->isOpen())
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
    netThread->terminate();

    delete window;
    delete netThread;
}

void Client::recieveMessage()
{
    while (!terminated)
    {
        MessageServer message;
        socket.recv(message);

        if(message.type == MessageServer::LOGIN){
            playerState.index = message.index;
        }

        world->copy(message.world);
        world->render();
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
    sf::Event event;
    bool processed = false;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            return false;
        processed = !playerState.handleEvent(event) || processed;
    }

    if(processed)
    {
        MessageClient messageClient;
        messageClient.type = MessageClient::MESSAGE;
        messageClient.nick = nick;
        messageClient.playerState = playerState;

        socket.send(messageClient, socket);
    }

    return true;
}
