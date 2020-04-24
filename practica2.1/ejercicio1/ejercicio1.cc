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
    if(argc != 2){
        printf("Numero de argumentos no valido\n");
        printf("Formato debe ser \"./program host\"\n");
        return -1;
    }
    // Opciones de filtrado
    struct addrinfo* result;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;

    // Result
    int rc = getaddrinfo(argv[1], NULL, &hints, &result);

    if (rc != 0) {
        std::cerr << "Error: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // Iteramos por cada estructura addrinfo
    while (result != nullptr) {
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];
        getnameinfo(result->ai_addr, result->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("IP: %s | FAMILIA: %d | TIPO DE SOCKET: %d\n", host, result->ai_family, result->ai_socktype);
        result = result->ai_next;
    }

    freeaddrinfo(result);

    return 0;
}