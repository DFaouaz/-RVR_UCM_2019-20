#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "Client.h"

class Server
{
public:
    Server();
    ~Server();

    void init();
    void run();
    void close();

private:
    void processClients() const;
    void processClient(Client* client) const;

private:
    // Atributos o propiedades
    // Vector de GameObjects?
    std::vector<Client*> clients;
    // Otros
};

#endif