#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include <SFML/Graphics.hpp>
#include "Socket.h"

#include "World.h"
#include "Player.h"

class MessageClient;
class PlayerState;

class Server
{
public:
    Server(const char * host, const char * port);
    ~Server();

    void init();
    void run();
    void close();

private:
    void manageMessages();
    void manageInput();

    void processLogin(Socket* client, const MessageClient& message);
    void processMessage(Socket* client, const MessageClient& message);
    void processLogout(Socket* client, const MessageClient& message);

    int clientExists(Socket* client);

    void addPlayer(int index);
    void removePlayer(int index);

    void sendWorld();
    int getFreeSlot();

    void createNetThread();

private:
    // Atributos o propiedades
    Socket socket;
    std::vector<std::thread> netThreads;
    std::thread inputThread;
    bool terminated;

    // Vector de GameObjects?
    std::vector<Socket*> clients;

    // Otros
    std::mutex worldMutex;
    std::mutex clientMutex;
    World* world;
    std::vector<Player*> players;
};

#endif