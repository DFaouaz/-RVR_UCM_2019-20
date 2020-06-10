#include "Server.h"
#include "MessageServer.h"
#include "MessageClient.h"

Server::Server(const char *host, const char *port) : socket(host, port), terminated(false)
{
}

Server::~Server()
{
}

// Public methods

void Server::init()
{
    // Socket bind
    socket.bind();

    // Init window (solo para probar logica, esto iria en Client)
    window = new sf::RenderWindow(sf::VideoMode(600, 480, 16), "SHUBE");

    // Init world
    world = new World(window);

    // Net thread
    netThread = std::thread(&Server::manageMessages, std::ref(*this));
}

void Server::run()
{
    sf::Clock timer;
    float deltaTime = 0;
    while (true)
    {
        // Procesar partida
        worldMutex.lock();
        world->render();
        world->update(deltaTime);
        worldMutex.unlock();

        sendWorld();

        deltaTime = timer.getElapsedTime().asSeconds();
        timer.restart();
    }
}

void Server::close()
{
    terminated = true;
    netThread.join();
    delete window;
}

// Private methods

void Server::manageMessages()
{
    // Mientras en hilo principal no este terminando
    while (!terminated)
    {
        // Recibir conexiones
        MessageClient message;
        Socket *client = &socket;
        if (socket.recv(message, client) < 0)
        {
            printf("Error al recibir mensaje\n");
            return; //continue;
        }

        switch (message.type)
        {
        case MessageClient::LOGIN:
            processLogin(client, message);
            break;
        case MessageClient::MESSAGE:
            processMessage(client, message);
            break;
        case MessageClient::LOGOUT:
            processLogout(client, message);
            break;
        default:
            break;
        }
    }
}

void Server::processLogin(Socket *client, const MessageClient &message)
{
    std::lock_guard<std::mutex> lock(clientMutex);
    // Comprobar si ya esta logeado
    for (auto c : clients)
    {
        if (*c == *client)
        {
            std::cout << "LOGGING ERROR: " << *client << "\n";
            delete client;
            client == nullptr;
        }
    }

    if (client == nullptr)
        return;

    // Si no, meterlo en un vector
    clients.push_back(client);
    std::cout << "CLIENT LOGGED: " << *client << "\n";

    //ECHO
    MessageServer messageServer;
    messageServer.type = MessageServer::LOGIN;
    messageServer.index = players.size();
    socket.send(messageServer, *client);

    addPlayer(players.size());

    // Crear otro hilo
    // TODO
}

void Server::processMessage(Socket *client, const MessageClient &message)
{
    std::lock_guard<std::mutex> lock(clientMutex);
    // Comprobar que existe el client en el pool
    int index = clientExists(client);
    if (index < 0)
        return;

    // Si es asi, modifica la info del cliente
    /*if (message.playerState.index < players.size())
        players[message.playerState.index];*/

    players[index]->processState(message.playerState);
}

void Server::processLogout(Socket *client, const MessageClient &message)
{
    std::lock_guard<std::mutex> lock(clientMutex);
    int index = clientExists(client);
    // No encontrado en clientes
    if (index < 0)
    {
        delete client;
        return;
    }
    std::cout << "CLIENT LOGGED OUT: " << *clients[index] << "\n";

    // Mensaje para que el cliente sepa que se ha podido desconectar
    MessageServer messageServer;
    messageServer.type = MessageServer::LOGOUT;
    socket.send(messageServer, *client);

    auto it = std::find(clients.begin(), clients.end(), clients[index]);
    delete clients[index];
    delete client;
    clients.erase(it);

    //Eliminar playerState
    //TODO

    //Remove player
    removePlayer(index);
}

int Server::clientExists(Socket *client)
{
    int i = 0;
    while (i < clients.size())
    {
        if (*clients[i] == *client)
            break;
        i++;
    }
    return i < clients.size() ? i : -1;
}

void Server::addPlayer(int index)
{
    std::lock_guard<std::mutex> lock(worldMutex);
    Player *player = new Player(index);
    world->addGameObject(player);

    players.push_back(player);
}

void Server::removePlayer(int index)
{
    std::lock_guard<std::mutex> lock(worldMutex);
    Player *player = players[index];
    world->removeGameObject(player);

    players.erase(players.begin() + index);
}

void Server::sendWorld()
{
    std::lock_guard<std::mutex> lock(clientMutex);
    // Estado del mundo
    MessageServer messageServer;
    messageServer.type = MessageServer::MESSAGE;
    messageServer.world.copy(*world);
    for (int i = 0; i < clients.size(); i++)
    {
        messageServer.index = i;
        socket.send(messageServer, *clients[i]);
    }
}