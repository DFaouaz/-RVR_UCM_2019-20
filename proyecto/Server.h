#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include "Socket.h"
#include "MessageClient.h"
#include "MessageServer.h"

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

    void processLogin(Socket* client, const MessageClient& message);
    void processMessage(Socket* client, const MessageClient& message);
    void processLogout(Socket* client, const MessageClient& message);

    int clientExists(Socket* client) const;

private:
    // Atributos o propiedades
    std::mutex clientsMutex;
    std::thread messagesThread;
    Socket socket;
    bool terminated;

    // Vector de GameObjects?
    std::vector<Socket*> clients;

    // Otros
    std::vector<PlayerState> playerStates;
    WorldState worldState;
};

#endif