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
    if(argc != 4){
        printf("Numero de argumentos no valido\n");
        printf("Formato debe ser \"./program host port command\"\n");
        return -1;
    }
    struct addrinfo hints;
    struct addrinfo* result;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);

    if (rc != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // Inicializacion y bind
    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // Apartado de envio de mensaje
    char* buffer = argv[3];
    sendto(sd, buffer, strlen(buffer) + 1, 0, result->ai_addr, result->ai_addrlen);

    // Apartado de recepcion de mensaje
    int size = 80;
    char recv_buffer[size];
    memset(&recv_buffer, '\0', size);
    ssize_t bytes = recvfrom(sd, recv_buffer, (size - 1) * sizeof(char), 0, result->ai_addr, &result->ai_addrlen);

    if (bytes == -1) {
        std::cerr << "recvfrom: " << std::endl;
        return -1;
    }

    printf("%s\n", recv_buffer);
    freeaddrinfo(result);

    return 0;
}