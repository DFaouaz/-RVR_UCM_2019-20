CC=g++
CFLAGS=-g -I.
DEPS = Server.h Client.h
OBJ = Server.o Client.o
LIBS=-lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) ServerProgram.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ClientProgram.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

