#include "Server.h"

Server::Server(const char *host, const char *port) : socket(host, port)
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

    // Hilo para recibir nuevas conexiones
    // something
}

void Server::run()
{
    // TODO: poner en un hilo desde init()
    manageMessages();

    // Procesar partida
}

void Server::close()
{
}

// Private methods

void Server::manageMessages()
{
    // Mientras en hilo principal no este terminando
    while (true)
    {
        // Recibir conexiones
        Message message;
        Socket *client;
        if (socket.recv(message, client) < 0)
        {
            printf("Error al recibir mensaje\n");
            continue;
        }

        switch (message.type)
        {
        case Message::LOGIN:
            processLogin(client, message);
            break;
        case Message::MESSAGE:
            processMessage(client, message);
            break;
        case Message::LOGOUT:
            processLogout(client, message);
            break;
        default:
            break;
        }
    }
}

void Server::processLogin(Socket *client, const Message &message)
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

    // Eco para que el cliente sepa que se ha podido conectar
    socket.send((Message &)message, *client);

    // Crear otro hilo
    // TODO
}

void Server::processMessage(Socket *client, const Message &message)
{
    // Comprobar que existe el client en el pool

    // Si es asi, modifica la info del cliente
}

void Server::processLogout(Socket *client, const Message &message)
{
    Socket *aux = nullptr;
    int i = 0;
    while (aux == nullptr && i < clients.size())
    {
        if (*clients[i] == *client)
            aux = clients[i];
        i++;
    }
    // No encontrado en clientes
    if (aux == nullptr)
    {
        delete client;
        return;
    }
    std::cout << "CLIENT LOGGED OUT: " << *aux << "\n";

    // Eco para que el cliente sepa que se ha podido desconectar
    socket.send((Message &)message, *aux);
    auto it = std::find(clients.begin(), clients.end(), aux);

    clients.erase(it);

    delete aux;
    delete client;
}