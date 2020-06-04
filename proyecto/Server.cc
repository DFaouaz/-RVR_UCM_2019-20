#include "Server.h"

Server::Server(const char *host, const char *port) : socket(host, port), terminated(false)
{
    worldState.nPlayers = 0;
    for (int i = 0; i < 4; i++)
        worldState.pPositions[i] = {0, 0};
}

Server::~Server()
{
}

// Public methods

void Server::init()
{
    // Socket bind
    socket.bind();

    // Hilo para recibir nuevas conexiones
    messagesThread = std::thread (&Server::manageMessages, std::ref(*this));
}

void Server::run()
{
    while (true)
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (int i = 0; i < clients.size(); i++)
        {
            MessageServer message;
            message.type = MessageServer::MESSAGE;
            message.worldState = worldState;

            socket.send(message, *clients[i]);
        }
    }

    // Procesar partida
}

void Server::close()
{
    terminated = true;
    messagesThread.detach();

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

        std::lock_guard<std::mutex> lock(clientsMutex);
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

    int nPlayer = ++worldState.nPlayers;
    worldState.pPositions[nPlayer - 1] = {50.0f * nPlayer, 50.0f * nPlayer};

    // Le manda el estado del juego
    MessageServer messageServer;
    messageServer.type = MessageServer::LOGIN;
    messageServer.worldState = worldState;

    socket.send(messageServer, *client);

    //Crea el playerState
    playerStates.push_back(message.playerState);

    // Crear otro hilo
    // TODO
}

void Server::processMessage(Socket *client, const MessageClient &message)
{
    // Comprobar que existe el client en el pool
    int index = clientExists(client);
    if(index < 0) return;

    // Si es asi, modifica la info del cliente
    if(index < playerStates.size())
        playerStates[index] = message.playerState;
}

void Server::processLogout(Socket *client, const MessageClient &message)
{
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
    messageServer.worldState = worldState;
    socket.send(messageServer, *client);
    auto it = std::find(clients.begin(), clients.end(), clients[index]);

    delete clients[index];
    delete client;
    clients.erase(it);

    //Eliminar playerState
    auto pIt = std::find(playerStates.begin(), playerStates.end(), playerStates[index]);
    playerStates.erase(pIt);
}

int Server::clientExists(Socket* client) const
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