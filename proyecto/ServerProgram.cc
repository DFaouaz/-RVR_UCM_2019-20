#include "Server.h"


int main(int argc, char *argv[])
{
    // Check arguments
    if(argc < 3) {
        printf("Formato debe de ser: ./program host port\n");
        return 0;
    }
    char* host = argv[1];
    char* port = argv[2];

    Server server(host, port);

    server.init();
    server.run();
    server.close();

    return 0;
}