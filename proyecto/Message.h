#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "Serializable.h"
#include "Socket.h"

struct PlayerState
{
    int xDirection;
    int yDirection;

    float xAim;
    float yAim;

    bool shooting;
};

class Message: public Serializable
{
public:
    static const size_t NICK_SIZE = 16; // El ultimo debe set \0
    static const size_t MESSAGE_SIZE = NICK_SIZE + sizeof(PlayerState);

    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2
    };

    Message();
    Message(const std::string& nick, const PlayerState& playerState);

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string nick;
    PlayerState playerState;
};
