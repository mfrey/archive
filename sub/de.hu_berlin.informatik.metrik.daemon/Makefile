CC = g++ -Wall -ansi -O3 -g 
BOOST_THREAD = -lboost_thread-mt
LOGGING = -llog4cxx
EXE = main

all: Daemon shell Configuration ConfigurationSettings Notification WatchDescriptorEntry main 

Daemon: Daemon.cpp 
	$(CC) $(LOGGING) -c Daemon.cpp

shell: shell.cpp
	$(CC) -c shell.cpp

Configuration: Configuration.cpp
	$(CC) -c Configuration.cpp

ConfigurationSettings: ConfigurationSettings.cpp
	$(CC) $(LOGGING) -c ConfigurationSettings.cpp

Notification: Notification.cpp
	$(CC) $(LOGGING) -c Notification.cpp

WatchDescriptorEntry: WatchDescriptorEntry.cpp
	$(CC) $(LOGGING) -c WatchDescriptorEntry.cpp

main: main.cpp Daemon.o Configuration.o shell.o Notification.o ConfigurationSettings.o WatchDescriptorEntry.o
	$(CC) main.cpp -o main Daemon.o Configuration.o shell.o Notification.o ConfigurationSettings.o WatchDescriptorEntry.o $(BOOST_THREAD) $(LOGGING) 

.cpp.obj:
	$(CC) -c $<

clean:
	rm *.o
	rm $(EXE)

