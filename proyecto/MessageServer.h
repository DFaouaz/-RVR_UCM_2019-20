#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "Serializable.h"
#include "World.h"


class MessageServer: public Serializable
{
public:
    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2
    };

    MessageServer();
    virtual ~MessageServer();

    void to_bin();

    int from_bin(char * data);

    uint8_t type;
    uint8_t index;
    World world;
};
