#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador : public Serializable
{
public:
    Jugador(const char *_n, int16_t _x, int16_t _y) : x(_x), y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        try
        {
            // Calcular espacio
            int32_t data_size = 80 + sizeof(int16_t) * 2;

            // Reservar memoria
            alloc_data(data_size);

            // Nombre
            memcpy(_data, name, 80);

            // Posicion (x, y)
            char *pos = _data + 80;
            memcpy(pos, (void *)&x, sizeof(int16_t));

            pos = pos + sizeof(int16_t);
            memcpy(pos, (void *)&y, sizeof(int16_t));
        }
        catch (std::exception exception)
        {
            printf("ERROR to_bin");
        }
    }

    int from_bin(char *data)
    {
        try
        {
            // Calcular espacio
            int32_t data_size = 80 + sizeof(int16_t) * 2;

            // Reservar memoria
            alloc_data(data_size);

            // Copiamos data
            memcpy(_data, data, data_size);

            // Nombre
            memcpy(name, data, 80);

            // Posicion (x, y)
            char *pos = data + 80;
            memcpy((void *)&x, pos, sizeof(int16_t));

            pos = pos + sizeof(int16_t);
            memcpy((void *)&y, pos, sizeof(int16_t));

            return 0;
        }
        catch (std::exception exception)
        {
            printf("ERROR from_bin");
        }
        return -1;
    }

public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // Serializar y escribir one_w en un fichero
    one_w.to_bin();

    // Open
    int fd = open("data", O_CREAT | O_WRONLY);
    if (fd == -1)
    {
        printf("Error al abrir archivo\n");
        return -1;
    }
    // Write de data
    ssize_t bytes = write(fd, one_w.data(), one_w.size());
    if (bytes == -1)
    {
        printf("Error al escribir en archivo\n");
        return -1;
    }
    // Cerrar
    int error = close(fd);
    if (error == -1)
    {
        printf("Error al cerrar descriptor de archivo\n");
        return -1;
    }

    // Leer el fichero en un buffer y "deserializar" en one_r
    // Open
    fd = open("data", O_RDONLY);
    if (fd == -1)
    {
        printf("Error al abrir archivo\n");
        return -1;
    }
    // Read de data
    char buffer[121];
    memset(buffer, '\0', 121);
    bytes = read(fd, buffer, 120);
    if (bytes == -1)
    {
        printf("Error al escribir en archivo\n");
        return -1;
    }
    // Cerrar
    close(fd);
    if (error == -1)
    {
        printf("Error al cerrar descriptor de archivo\n");
        return -1;
    }
    // Desserializar
    one_r.from_bin(buffer);

    // Mostrar el contenido one_r
    printf("%s %d %d\n", one_r.name, one_r.x, one_r.y);

    return 0;
}

/* RESPUESTAS DEL ENUNCIADO */

/*
    - El comando od muestra en terminal el contenido del fichero en octal u otros formatos
    - El tamaño de data serializado, es el mismo que el data que se muestra con el comando od.
*/