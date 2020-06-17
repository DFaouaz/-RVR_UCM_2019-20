#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "Serializable.h"
#include "PlayerState.h"


class MessageClient : public Serializable
{
public:
    static const size_t NICK_SIZE = 16; // El ultimo debe set \0
    static const size_t MESSAGE_SIZE = NICK_SIZE + sizeof(PlayerState);

    enum MessageType
    {
        LOGIN = 0,
        MESSAGE = 1,
        LOGOUT = 2,
        NONE = 3
    };

    MessageClient();
    MessageClient(const std::string &nick, const PlayerState &playerState);
    virtual ~MessageClient();

    void to_bin();

    int from_bin(char *bobj);

    MessageType type;

    std::string nick;
    PlayerState playerState;
};
