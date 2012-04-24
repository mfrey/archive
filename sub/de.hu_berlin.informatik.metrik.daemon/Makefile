CC = g++ -Wall -Werror -ansi -pedantic -O3 -g 
BOOST_THREAD = -lboost_thread
BOOST_SYSTEM = -lboost_system
LOGGING = -llog4cxx
CPPUNIT = -lcppunit
PTHREAD = -lpthread
EXE = main

all: main 

test: WatchDescriptorEntryTest.o Tests.o WatchDescriptorEntry.o Notification.o NotificationTest.o Actor.o Telnet.o TelnetActor.o TelnetActorTest.o
	$(CC) -o test WatchDescriptorEntryTest.o Tests.o WatchDescriptorEntry.o Notification.o NotificationTest.o Actor.o Telnet.o TelnetActor.o TelnetActorTest.o $(CPPUNIT) $(LOGGING) $(BOOST_THREAD) $(PTHREAD) $(BOOST_SYSTEM)

Tests.o: Tests.cpp
	$(CC) -c Tests.cpp $(CPPUNIT)

Daemon.o: Daemon.cpp 
	$(CC) $(LOGGING) -c Daemon.cpp 

Shell.o: Shell.cpp
	$(CC) -c Shell.cpp

Actor.o: Actor.cpp
	$(CC) -c Actor.cpp

TelnetActor.o: TelnetActor.cpp
	$(CC) $(LOGGING) -c TelnetActor.cpp

TelnetActorTest.o: TelnetActorTest.cpp
	$(CC) $(LOGGING) -c TelnetActorTest.cpp

Telnet.o: Telnet.cpp
	$(CC) $(LOGGING) -c Telnet.cpp

Sensor.o: Sensor.cpp
	$(CC) -c Sensor.cpp

Controller.o: Controller.cpp
	$(CC) -c Controller.cpp

Configuration.o: Configuration.cpp
	$(CC) -c Configuration.cpp

ConfigurationSettings.o: ConfigurationSettings.cpp
	$(CC) $(LOGGING) -c ConfigurationSettings.cpp

Notification.o: Notification.cpp
	$(CC) $(LOGGING) -c Notification.cpp

WatchDescriptorEntry.o: WatchDescriptorEntry.cpp
	$(CC) $(LOGGING) -c WatchDescriptorEntry.cpp

WatchDescriptorEntryTest.o: WatchDescriptorEntryTest.cpp
	$(CC) -c WatchDescriptorEntryTest.cpp

main: main.cpp Daemon.o Configuration.o Shell.o Notification.o ConfigurationSettings.o WatchDescriptorEntry.o Actor.o Sensor.o Controller.o TelnetActor.o Telnet.o
	$(CC) main.cpp -o $(EXE) Daemon.o Configuration.o Shell.o Notification.o ConfigurationSettings.o WatchDescriptorEntry.o Actor.o Sensor.o Controller.o TelnetActor.o Telnet.o $(BOOST_THREAD) $(LOGGING) $(BOOST_SYSTEM) $(PTHREAD)

.cpp.obj:
	$(CC) -c $<

clean:
	rm -f *.o
	rm -f $(EXE)

