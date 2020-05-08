// Anson Alcolea, Alejandro
// Faouaz Santillana, Dany

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>
#include <thread>

#include <iostream>

#include <time.h>

#define NUM_THREADS 5

class MessageManager
{
public:
    MessageManager(int socket_descriptor) : socket_descriptor(socket_descriptor), terminated(false)
    {
    }

    ~MessageManager()
    {
        terminated = true;
    }

    void manage()
    {
        // Size = 2 para la primera letra y el \0
        int size = 2;
        char buffer[size];
        memset(&buffer, '\0', sizeof(char) * size); // Inicializamos el buffer
        // Espera a que el la letra del buffer sea 'q' para salir
        while (!terminated)
        {
            // Apartado de recepcion de mensaje
            char host[NI_MAXHOST];
            char service[NI_MAXSERV];
            memset(&host, '\0', sizeof(char) * NI_MAXHOST);
            memset(&service, '\0', sizeof(char) * NI_MAXSERV);

            struct sockaddr client_addr;
            socklen_t client_len = sizeof(struct sockaddr);

            // Limpiamos el buffer
            memset(&buffer, '\0', sizeof(char) * size);

            // FLAG no espera mensaje
            ssize_t bytes = recvfrom(socket_descriptor, buffer, (size - 1) * sizeof(char), MSG_DONTWAIT, &client_addr, &client_len);

            if (bytes == -1)
            {
                continue;
            }

            buffer[bytes] = '\0';

            getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
            printf("%d bytes from %s:%s | THREAD ID: ", (int)bytes, host, service);
            std::cout << std::this_thread::get_id() << "\n";

            // Apartado de respuesta
            time_t t = time(NULL);
            tm *date_time = localtime(&t);
            // Buffer del mensaje de respuesta
            int send_size = 20;
            char send_buffer[send_size];
            memset(&send_buffer, '\0', sizeof(char) * send_size);

            // Tiempo
            if (buffer[0] == 't')
            {
                ssize_t send_bytes = strftime(send_buffer, 20, "%r", date_time);
                sendto(socket_descriptor, send_buffer, send_bytes, 0, &client_addr, client_len);
            }
            // Fecha
            else if (buffer[0] == 'd')
            {
                ssize_t send_bytes = strftime(send_buffer, 20, "%F", date_time);
                sendto(socket_descriptor, send_buffer, send_bytes, 0, &client_addr, client_len);
            }
            else
            {
                sendto(socket_descriptor, "\0", 1, 0, &client_addr, client_len);
                printf("Comando no valido %s\n", buffer);
            }
            sleep(3);
        }
    }

private:
    int socket_descriptor;

public:
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
    hints.ai_socktype = SOCK_DGRAM;

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

    // POOL DE THREADS
    std::vector<std::thread> threads;
    std::vector<MessageManager *> managers;

    printf("CREANDO THREADS\n");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        std::shared_ptr<MessageManager> manager(new MessageManager(sd));
        threads.push_back(std::thread(&MessageManager::manage, manager));
        managers.push_back(manager.get());
        std::cout << "Thread creado | ID: " << threads[i].get_id() << "\n";
    }
    printf("THREADS CREADOS\n");

    std::string user_input = "none";
    do
    {
        std::cin >> user_input;
    } while (strcmp(user_input.c_str(), "q"));

    printf("CERRANDO THREADS\n");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        std::thread::id id = threads[i].get_id();
        managers[i]->terminated = true;
        threads[i].join();
        std::cout << "Thread cerrado | ID: " << id << "\n";
    }
    printf("THREADS CERRADOS\n");

    return 0;
}