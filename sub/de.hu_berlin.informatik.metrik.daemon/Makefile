CC = g++ -Wall -ansi -O3 -g 
BOOST_THREAD = -lboost_thread-mt
LOGGING = -llog4cxx
EXE = main

all: daemon shell configuration notification main

daemon: daemon.cpp
	$(CC) -c daemon.cpp

shell: shell.cpp
	$(CC) -c shell.cpp

configuration: configuration.cpp
	$(CC) -c configuration.cpp

notification: notification.cpp
	$(CC) $(LOGGING) -c notification.cpp

main: main.cpp daemon.o configuration.o shell.o notification.o
	$(CC) main.cpp $(BOOST_THREAD) $(LOGGING) -o main daemon.o configuration.o shell.o notification.o

.cpp.obj:
	$(CC) -c $<

clean:
	rm *.o
	rm $(EXE)

