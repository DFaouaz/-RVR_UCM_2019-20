#include "MessageClient.h"

MessageClient::MessageClient()
{
}

MessageClient::MessageClient(const std::string &nick, const PlayerState &playerState) : nick(nick), playerState(playerState)
{
}

MessageClient::~MessageClient()
{
}

void MessageClient::to_bin()
{
    alloc_data(sizeof(MessageType) + NICK_SIZE + sizeof(PlayerState));

    memset(_data, 0, sizeof(MessageType) + NICK_SIZE + sizeof(PlayerState));

    // Serializar los campos type, nick y message en el buffer _data
    // Type
    memcpy(_data, (void *)&type, sizeof(type));

    // Nick
    char *pos = _data + sizeof(type);
    nick[NICK_SIZE - 1] = '\0';
    memcpy(pos, nick.c_str(), NICK_SIZE);

    //PlayerState Index
    pos += NICK_SIZE;
    memcpy(pos, &playerState.index, sizeof(playerState.index));

    //PlayerState xDirection
    pos += sizeof(playerState.index);
    memcpy(pos, &playerState.xDirection, sizeof(playerState.xDirection));

    //PlayerState yDirection
    pos += sizeof(playerState.xDirection);
    memcpy(pos, &playerState.yDirection, sizeof(playerState.yDirection));

    //PlayerState xAim
    pos += sizeof(playerState.yDirection);
    memcpy(pos, &playerState.xAim, sizeof(playerState.xAim));

    //PlayerState yAim
    pos += sizeof(playerState.xAim);
    memcpy(pos, &playerState.yAim, sizeof(playerState.yAim));

    //PlayerState Shooting
    pos += sizeof(playerState.yAim);
    memcpy(pos, &playerState.shooting, sizeof(playerState.shooting));
}

int MessageClient::from_bin(char *data)
{
    try
    {
        alloc_data(sizeof(MessageType) + NICK_SIZE + sizeof(PlayerState));

        memcpy(static_cast<void *>(_data), data, sizeof(MessageType) + NICK_SIZE + sizeof(PlayerState));

        //Reconstruir la clase usando el buffer _data
        // Type
        memcpy((void *)&type, _data, sizeof(type));

        // Nick
        char *pos = _data + sizeof(type);
        char name[NICK_SIZE];
        memcpy(name, pos, NICK_SIZE);
        name[NICK_SIZE - 1] = '\0';
        nick = name;

        //PlayerState Index
        pos += NICK_SIZE;
        memcpy(&playerState.index, pos, sizeof(playerState.index));

        //PlayerState xDirection
        pos += sizeof(playerState.index);
        memcpy(&playerState.xDirection, pos, sizeof(playerState.xDirection));

        //PlayerState yDirection
        pos += sizeof(playerState.xDirection);
        memcpy(&playerState.yDirection, pos, sizeof(playerState.yDirection));

        //PlayerState xAim
        pos += sizeof(playerState.yDirection);
        memcpy(&playerState.xAim, pos, sizeof(playerState.xAim));

        //PlayerState yAim
        pos += sizeof(playerState.xAim);
        memcpy(&playerState.yAim, pos, sizeof(playerState.yAim));

        //PlayerState Shooting
        pos += sizeof(playerState.yAim);
        memcpy(&playerState.shooting, pos, sizeof(playerState.shooting));

        return 0;
    }
    catch (std::exception exception)
    {
    }

    return -1;
}