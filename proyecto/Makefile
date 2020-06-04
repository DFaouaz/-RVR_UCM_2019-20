CC = g++
CFLAGS = -g -I.
DEPS = Server.h Client.h MessageClient.h MessageServer.h Socket.h
OBJ = Server.o Client.o MessageClient.o MessageServer.o Socket.o
LIBS = -lpthread -lSDL2 `sdl2-config --cflags --libs`

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LIBS)

all: ServerProgram ClientProgram

ServerProgram: $(OBJ) ServerProgram.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

ClientProgram: $(OBJ) ClientProgram.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o ServerProgram ClientProgram

