CC = g++ -Wall -ansi -O3 -g 
BOOST_THREAD = -lboost_thread
LOGGING = -llog4cxx
CPPUNIT = -lcppunit
EXE = main

all: Daemon Shell ConfigurationSettings Configuration Notification WatchDescriptorEntry Actor Sensor Controller main 

Test: WatchDescriptorEntryTest.o Tests.o WatchDescriptorEntry.o Notification.o NotificationTest.o
	$(CC) -o test WatchDescriptorEntryTest.o Tests.o WatchDescriptorEntry.o Notification.o NotificationTest.o $(CPPUNIT) $(LOGGING) $(BOOST_THREAD)

Tests: Tests.cpp
	$(CC) -c Tests.cpp $(CPPUNIT)

Daemon: Daemon.cpp 
	$(CC) $(LOGGING) -c Daemon.cpp 

Shell: Shell.cpp
	$(CC) -c Shell.cpp

Actor: Actor.cpp
	$(CC) -c Actor.cpp

Sensor: Sensor.cpp
	$(CC) -c Sensor.cpp

Controller: Controller.cpp
	$(CC) -c Controller.cpp

Configuration: Configuration.cpp
	$(CC) -c Configuration.cpp

ConfigurationSettings: ConfigurationSettings.cpp
	$(CC) $(LOGGING) -c ConfigurationSettings.cpp

Notification: Notification.cpp
	$(CC) $(LOGGING) -c Notification.cpp

WatchDescriptorEntry: WatchDescriptorEntry.cpp
	$(CC) $(LOGGING) -c WatchDescriptorEntry.cpp

main: main.cpp Daemon.o Configuration.o Shell.o Notification.o ConfigurationSettings.o WatchDescriptorEntry.o Actor.o Sensor.o Controller.o
	$(CC) main.cpp -o main Daemon.o Configuration.o Shell.o Notification.o ConfigurationSettings.o WatchDescriptorEntry.o Actor.o Sensor.o Controller.o $(BOOST_THREAD) $(LOGGING) 

.cpp.obj:
	$(CC) -c $<

clean:
	rm *.o
	rm $(EXE)

