#include "Client.h"


int main(int argc, char *argv[])
{
    // Check arguments
     // Check arguments
    if(argc != 4) {
        printf("Formato debe de ser: ./program host port nick\n");
        return 0;
    }
    char* host = argv[1];
    char* port = argv[2];
    char* nick = argv[3];

    Client client(nick, host, port);

    client.init();
    client.run();
    client.close();

    return 0;
}