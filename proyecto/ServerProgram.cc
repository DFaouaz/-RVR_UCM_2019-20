#include "Server.h"


int main(int argc, char *argv[])
{
    // Check arguments
    // TODO
    // ...


    Server server;

    server.init();
    server.run();
    server.close();

    return 0;
}