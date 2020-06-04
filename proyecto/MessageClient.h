#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "Serializable.h"

struct PlayerState
{
    PlayerState() : xDirection(0), yDirection(0), xAim(0.0f), yAim(0.0f), shooting(false) {}
    int xDirection;
    int yDirection;

    float xAim;
    float yAim;

    bool shooting;

    bool operator==(const PlayerState& other) const
    {
        return  xDirection == other.xDirection &&
                yDirection == other.yDirection &&
                xAim == other.xAim &&
                yAim == other.yAim &&
                shooting == other.shooting;
    }
};

class MessageClient: public Serializable
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

    MessageClient();
    MessageClient(const std::string& nick, const PlayerState& playerState);
    virtual ~MessageClient();

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string nick;
    PlayerState playerState;
};
