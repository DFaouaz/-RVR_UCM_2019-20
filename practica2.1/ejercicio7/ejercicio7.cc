// Anson Alcolea, Alejandro
// Faouaz Santillana, Dany

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <thread>
#include <memory>

#include <iostream>

class ConnectionManager
{
public:
    ConnectionManager(int socket_descriptor) : socket_descriptor(socket_descriptor), terminated(false)
    {
    }

    ~ConnectionManager()
    {
        terminated = true;
    }

    void manage()
    {
        int size = 120;
        char buffer[size];
        memset(&buffer, '\0', sizeof(char) * size);

        ssize_t bytes = -1;
        while (!terminated)
        {
            // Limpieza de buffer
            memset(&buffer, '\0', sizeof(char) * size);
            // Recepcion
            bytes = recv(socket_descriptor, buffer, (size - 1) * sizeof(char), 0);

            if (bytes == -1)
            {
                continue;
            }

            if (bytes == 0)
            {
                printf("Conexion terminada\n");
                terminated = true;
                continue;
            }

            buffer[bytes] = '\0';

            // Respuesta
            bytes = send(socket_descriptor, buffer, bytes, 0);

            if (bytes == -1)
            {
                std::cerr << "send: " << std::endl;
                return;
            }
            sleep(3);
        }
    }

private:
    int socket_descriptor;
    bool terminated;
};

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

    if (bind(sd, result->ai_addr, result->ai_addrlen) != 0)
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    freeaddrinfo(result);

    // Publicamos el servidor (listen)
    if (listen(sd, 16) == -1)
    {
        std::cerr << "listen: " << std::endl;
        return -1;
    }

    while (true)
    {
        // Gestionar conexion (accept)
        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);
        int client_sd = accept(sd, &client_addr, &client_len);

        if (client_sd == -1)
        {
            std::cerr << "accept: " << std::endl;
            return -1;
        }

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        printf("Conexion desde %s %s\n", host, service);

        // Creacion del thread
        std::shared_ptr<ConnectionManager> connection(new ConnectionManager(client_sd));

        std::thread thread(&ConnectionManager::manage, connection);
        thread.detach();
    }

    return 0;
}