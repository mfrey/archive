CC = g++ -Wall -ansi  -O3
EXE = main

all: daemon logging configuration main

daemon: daemon.cpp
	$(CC) -c daemon.cpp

logging: logging.cpp
	$(CC) -c logging.cpp

configuration: configuration.cpp
	$(CC) -c configuration.cpp

main: main.cpp daemon.o logging.o configuration.o
	$(CC) main.cpp -o main daemon.o logging.o configuration.o

.cpp.obj:
	$(CC) -c $<

clean:
	rm *.o
	rm $(EXE)

