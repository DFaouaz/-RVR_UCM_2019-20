// Anson Alcolea, Alejandro
// Faouaz Santillana, Dany

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char **argv)
{
    // Gestion de errores
    if (argc != 3)
    {
        printf("Numero de argumentos no valido\n");
        printf("Formato debe ser \"./program host port\"\n");
        return -1;
    }
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);

    if (rc != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // Inicializacion y bind
    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // Conexion
    if (connect(sd, result->ai_addr, result->ai_addrlen) == -1)
    {
        std::cerr << "connect: " << std::endl;
        return -1;
    };

    int size = 120;
    char buffer[size];
    memset(&buffer, '\0', sizeof(char) * size);

    ssize_t bytes = -1;
    while (bytes != 0)
    {
        memset(&buffer, '\0', sizeof(char) * size);
        std::cin >> buffer;
        if (!strcmp(buffer, "Q"))
        {
            break;
        }

        bytes = send(sd, buffer, strlen(buffer) + 1, 0);
        if (bytes == -1)
        {
            std::cerr << "send: " << std::endl;
            return -1;
        }

        char recv_buffer[size];
        memset(&recv_buffer, '\0', size);
        bytes = recv(sd, recv_buffer, (size - 1) * sizeof(char), 0);
        if (bytes == -1)
        {
            std::cerr << "recv: " << std::endl;
            return -1;
        }
        if (bytes == 0)
        {
            continue;
        }
        recv_buffer[bytes] = '\0';

        printf("%s\n", recv_buffer);
    }
    freeaddrinfo(result);

    return 0;
}