CC = g++ -Wall -ansi -O3 -g 
BOOST_THREAD = -lboost_thread-mt
LOGGING = -llog4cxx
EXE = main

all: Daemon shell configuration Notification settings WatchDescriptorEntry main 

Daemon: Daemon.cpp 
	$(CC) -c Daemon.cpp

shell: shell.cpp
	$(CC) -c shell.cpp

configuration: configuration.cpp
	$(CC) -c configuration.cpp

settings: settings.cpp
	$(CC) $(LOGGING) -c settings.cpp

Notification: Notification.cpp
	$(CC) $(LOGGING) -c Notification.cpp

WatchDescriptorEntry: WatchDescriptorEntry.cpp
	$(CC) $(LOGGING) -c WatchDescriptorEntry.cpp

main: main.cpp Daemon.o configuration.o shell.o Notification.o settings.o WatchDescriptorEntry.o
	$(CC) main.cpp $(BOOST_THREAD) $(LOGGING) -o main Daemon.o configuration.o shell.o Notification.o settings.o WatchDescriptorEntry.o

.cpp.obj:
	$(CC) -c $<

clean:
	rm *.o
	rm $(EXE)

