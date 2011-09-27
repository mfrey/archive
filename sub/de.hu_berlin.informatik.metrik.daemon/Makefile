CC = g++ -Wall -ansi -O3 -g 
BOOST_THREAD = -lboost_thread-mt
EXE = main

all: daemon logging shell configuration notification main

daemon: daemon.cpp
	$(CC) -c daemon.cpp

logging: logging.cpp
	$(CC) -c logging.cpp

shell: shell.cpp
	$(CC) -c shell.cpp

configuration: configuration.cpp
	$(CC) -c configuration.cpp

notification: notification.cpp
	$(CC) -c notification.cpp

main: main.cpp daemon.o logging.o configuration.o shell.o notification.o
	$(CC) main.cpp $(BOOST_THREAD) -o main daemon.o logging.o configuration.o shell.o notification.o

.cpp.obj:
	$(CC) -c $<

clean:
	rm *.o
	rm $(EXE)

