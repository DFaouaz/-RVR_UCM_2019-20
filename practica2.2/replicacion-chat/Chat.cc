#include "Chat.h"

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    // Serializar los campos type, nick y message en el buffer _data
    // Type
    memcpy(_data, (void *)&type, sizeof(type));

    // Nick
    char *pos = _data + sizeof(type);
    nick[7] = '\0';
    memcpy(pos, nick.c_str(), 8);

    // Message
    pos = pos + 8;
    message[79] = '\0';
    memcpy(pos, message.c_str(), 80);
}

int ChatMessage::from_bin(char *bobj)
{
    try
    {
        alloc_data(MESSAGE_SIZE);

        memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

        //Reconstruir la clase usando el buffer _data
        // Type
        memcpy((void *)&type, _data, sizeof(type));

        // Nick
        char *pos = _data + sizeof(type);
        char name[8];
        memcpy(name, pos, 8);
        name[7] = '\0';
        nick = name;

        // Message
        pos = pos + 8;
        char message[80];
        memcpy(message, pos, 80);
        message[79] = '\0';
        this->message = message;

        return 0;
    }
    catch (std::exception exception)
    {
    }

    return -1;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        ChatMessage *obj = new ChatMessage();
        Socket *client = &socket;
        socket.recv(*obj, client);

        // LOGIN
        if (obj->type == ChatMessage::LOGIN)
        {
            for (Socket *c : clients)
            {
                if (*c == *client)
                {
                    std::cout << "LOGING ERROR: " << *client << "\n";
                    delete client;
                    client == nullptr;
                }
            }

            if (client == nullptr)
                continue;

            clients.push_back(client);
            std::cout << "CLIENT LOGED: " << *client << "\n";
        }
        // LOGOUT
        else if (obj->type == ChatMessage::LOGOUT)
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
                continue;
            }
            else
            {
                socket.send(*obj, *aux);
                auto it = std::find(clients.begin(), clients.end(), aux);
                clients.erase(it);

                std::cout << "CLIENT LOGED OUT: " << *aux << "\n";

                delete aux;
                delete client;
            }
        }
        // MESSAGE
        else if (obj->type == ChatMessage::MESSAGE)
        {
            for (auto *c : clients)
            {
                if (*c == *client)
                    continue;
                socket.send(*obj, *c);
            }
            delete client;
            printf("Enviando \"%s\" a todos los usuarios menos al remitente\n", obj->message.c_str());
        }
    }
}

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        std::string msg;
        if (!std::getline(std::cin, msg))
            break;

        if (!strcmp(msg.c_str(), "logout"))
            break;

        // Enviar al servidor usando socket
        ChatMessage em(nick, msg);
        em.type = ChatMessage::MESSAGE;

        socket.send(em, socket);
    }
}

void ChatClient::net_thread()
{
    while (true)
    {
        // Recibir Mensajes de red
        std::string msg;
        ChatMessage em;
                
        if (socket.recv(em) < 0)
            break;
        if (em.type == ChatMessage::LOGOUT)
            break;

        // Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        printf("%s: %s\n", em.nick.c_str(), em.message.c_str());
    }
}
