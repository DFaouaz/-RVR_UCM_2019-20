CC = g++
CFLAGS = -g -I.
DEPS = Server.h Client.h MessageClient.h MessageServer.h Socket.h World.h GameObject.h Player.h Bullet.h PlayerState.h
OBJ = Server.o Client.o MessageClient.o MessageServer.o Socket.o World.o GameObject.o Player.o Bullet.o PlayerState.o
LIBS = -lpthread -lsfml-graphics -lsfml-window -lsfml-system -lX11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LIBS)

all: server client

server: $(OBJ) ServerProgram.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ) ClientProgram.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o server client

