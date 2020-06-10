#include "MessageServer.h"

MessageServer::MessageServer() : world(nullptr)
{

}

MessageServer::~MessageServer()
{

}

void MessageServer::to_bin()
{
    world.to_bin();

    alloc_data(sizeof(type) + sizeof(index) + world.size());

    memset(_data, 0, sizeof(type) + sizeof(index) + world.size());

    // Serializar los campos type y message en el buffer _data
    // Type
    memcpy(_data, &type, sizeof(type));

    // Index
    char *pos = _data + sizeof(type);
    memcpy(pos, &index, sizeof(index));

    // World
    pos += sizeof(index);
    memcpy(pos, world.data(), world.size());
}

int MessageServer::from_bin(char *data)
{
    try
    {
        //Reconstruir la clase usando el buffer _data
        // Type
        memcpy(&type, data, sizeof(type));

        // Index
        char *pos = data + sizeof(type);
        memcpy(&index, pos, sizeof(index));

        // World
        pos += sizeof(index);
        world.from_bin(pos);

        return 0;
    }
    catch (std::exception exception)
    {
    }

    return -1;
}