// Anson Alcolea, Alejandro
// Faouaz Santillana, Dany 

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

#include <time.h>


int main(int argc, char **argv)
{
    // Gestion de errores
    if(argc != 3){
        printf("Numero de argumentos no valido\n");
        printf("Formato debe ser \"./program host port\"\n");
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

    if (bind(sd, result->ai_addr, result->ai_addrlen) != 0) {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    freeaddrinfo(result);

    int size = 2;
    char buffer[size];
    memset(&buffer, '\0', sizeof(char) * size);
    // Espera a que el la letra del buffer sea 'q' para salir
    while(buffer[0] != 'q') {
        // Apartado de recepcion de mensaje
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);

        ssize_t bytes = recvfrom(sd, buffer, (size - 1) * sizeof(char), 0, &client_addr, &client_len);

        if (bytes == -1) {
            std::cerr << "recvfrom: " << std::endl;
            return -1;
        }

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("%d bytes from %s:%s\n", (int)bytes, host, service);

        // Apartado de respuesta 
        time_t t = time(NULL);
        tm* date_time = localtime(&t);
        // Buffer del mensaje de respuesta
        int send_size = 20;
        char send_buffer[send_size];
        memset(&send_buffer, '\0', sizeof(char) * send_size);

        // Tiempo
        if(buffer[0] == 't') {
            ssize_t send_bytes = strftime(send_buffer, 20, "%r", date_time);
            sendto(sd, send_buffer, send_bytes, 0, &client_addr, client_len);
        }
        // Fecha
        else if (buffer[0] == 'd') {
            ssize_t send_bytes = strftime(send_buffer, 20, "%F", date_time);
            sendto(sd, send_buffer, send_bytes, 0, &client_addr, client_len);
        }
        else {
            sendto(sd, "", 1, 0, &client_addr, client_len);
        }
    }

    return 0;
}