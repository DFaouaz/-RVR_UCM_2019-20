#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include "Socket.h"
#include "Message.h"
#include "Client.h"

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

    void processLogin(Socket* client, const Message& message);
    void processMessage(Socket* client, const Message& message);
    void processLogout(Socket* client, const Message& message);

private:
    // Atributos o propiedades
    Socket socket;

    // Vector de GameObjects?
    std::vector<Socket*> clients;
    // Otros
};

#endif