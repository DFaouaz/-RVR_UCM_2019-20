#include "MessageServer.h"

MessageServer::MessageServer()
{
}

MessageServer::MessageServer(const WorldState &worldState)
{
}

MessageServer::~MessageServer()
{

}

void MessageServer::to_bin()
{
    alloc_data(sizeof(MessageType) + MESSAGE_SIZE);

    memset(_data, 0, sizeof(MessageType) + MESSAGE_SIZE);

    // Serializar los campos type y message en el buffer _data
    // Type
    memcpy(_data, &type, sizeof(type));

    // WorldState
    char *pos = _data + sizeof(type);
    memcpy(pos, &worldState, MESSAGE_SIZE);
}

int MessageServer::from_bin(char *data)
{
     try
    {
        alloc_data(sizeof(MessageType) + MESSAGE_SIZE);

        memcpy(static_cast<void *>(_data), data, sizeof(MessageType) + MESSAGE_SIZE);

        //Reconstruir la clase usando el buffer _data
        // Type
        memcpy(&type, _data, sizeof(type));

        // WorldState
        char *pos = _data + sizeof(type);
        memcpy(&worldState, pos, MESSAGE_SIZE);

        return 0;
    }
    catch (std::exception exception)
    {
    }

    return -1;
}