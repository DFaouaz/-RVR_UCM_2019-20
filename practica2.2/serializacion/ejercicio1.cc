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
        // Calcular espacio
        int32_t data_size = 80 + sizeof(int16_t) * 2;

        // Reservar memoria
        alloc_data(data_size);

        // Nombre
        strcpy(_data, name);

        // Posicion (x, y)
        char *pos = _data + 80;
        strncpy(pos, (char *)&x, sizeof(int16_t));

        pos = pos + sizeof(int16_t);
        strncpy(pos, (char *)&y, sizeof(int16_t));
    }

    int from_bin(char *data)
    {
        try
        {
            // Calcular espacio
            int32_t data_size = 80 + sizeof(int16_t) * 2;

            // Reservar memoria
            alloc_data(data_size);

            // Nombre
            strncpy(name, data, 80);

            // Posicion (x, y)
            char *pos = data + 80;
            strncpy((char *)&x, pos, sizeof(int16_t));

            pos = pos + sizeof(int16_t);
            strncpy((char *)&y, pos, sizeof(int16_t));

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
    // Write de data
    write(fd, one_w.data(), one_w.size());
    // Cerrar
    close(fd);

    // Leer el fichero en un buffer y "deserializar" en one_r
    // Open
    fd = open("data", O_RDONLY);
    // Read de data
    char buffer[121];
    memset(buffer, '\0', 121);
    read(fd, buffer, 120);
    // Cerrar
    close(fd);
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