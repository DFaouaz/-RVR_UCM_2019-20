#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "Serializable.h"

struct Position {
    float x;
    float y;
};

struct WorldState
{
    // Players
    int nPlayers;
    Position pPositions[4];
};

class MessageServer: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(WorldState);

    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2
    };

    MessageServer();
    MessageServer(const WorldState& worldState);
    virtual ~MessageServer();

    void to_bin();

    int from_bin(char * data);

    uint8_t type;
    WorldState worldState;
};
