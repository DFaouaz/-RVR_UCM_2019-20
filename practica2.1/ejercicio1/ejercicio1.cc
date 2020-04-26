// Anson Alcolea, Alejandro
// Faouaz Santillana, Dany

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>

int main(int argc, char **argv)
{
    // Gestion de errores
    if (argc != 2)
    {
        printf("Numero de argumentos no valido\n");
        printf("Formato debe ser \"./program host\"\n");
        return -1;
    }
    // Opciones de filtrado
    struct addrinfo *result;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;

    // Result
    int rc = getaddrinfo(argv[1], NULL, &hints, &result);

    if (rc != 0)
    {
        std::cerr << "Error: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // Iteramos por cada estructura addrinfo
    struct addrinfo *aux_result = result;
    while (aux_result != nullptr)
    {
        char host[NI_MAXHOST];
        getnameinfo(aux_result->ai_addr, aux_result->ai_addrlen, host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("IP: %24s | FAMILIA: %2d | TIPO DE SOCKET: %d\n", host, aux_result->ai_family, aux_result->ai_socktype);
        aux_result = aux_result->ai_next;
    }

    freeaddrinfo(result);

    return 0;
}