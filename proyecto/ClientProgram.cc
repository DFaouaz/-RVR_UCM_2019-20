#include "Client.h"


int main(int argc, char *argv[])
{
    // Check arguments
    // TODO
    // ...


    Client client;

    client.init();
    client.run();
    client.close();

    return 0;
}