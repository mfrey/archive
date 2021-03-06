CC = g++ -Wall -Werror -ansi -pedantic -O3 -g
BOOST_THREAD = -lboost_thread
BOOST_SYSTEM = -lboost_system
LOGGING = -llog4cxx
CPPUNIT = -lcppunit
PTHREAD = -lpthread
OBJECTS = Daemon.o Configuration.o Shell.o Notification.o WatchDescriptorEntry.o Actor.o Sensor.o Controller.o TelnetActor.o Telnet.o Socket.o TelnetSocket.o
TEST_OBJECTS = WatchDescriptorEntryTest.o Tests.o WatchDescriptorEntry.o Notification.o NotificationTest.o Actor.o Telnet.o TelnetActor.o TelnetActorTest.o DequeTest.o Configuration.o ConfigurationTest.o SocketTest.o Socket.o
EXE = main
TEST_EXE = test

all: main 

test: $(TEST_OBJECTS)
	$(CC) -o $(TEST_EXE) $(TEST_OBJECTS) $(CPPUNIT) $(LOGGING) $(BOOST_THREAD) $(PTHREAD) $(BOOST_SYSTEM)

Tests.o: Tests.cpp
	$(CC) -c Tests.cpp $(CPPUNIT)

Daemon.o: Daemon.cpp 
	$(CC) $(LOGGING) -c Daemon.cpp 

Socket.o: Socket.cpp 
	$(CC) -c Socket.cpp $(LOGGING)

SocketTest.o: SocketTest.cpp 
	$(CC) -c SocketTest.cpp $(LOGGING)

DequeTest.o: DequeTest.cpp
	$(CC) -c DequeTest.cpp

Shell.o: Shell.cpp
	$(CC) -c Shell.cpp

Actor.o: Actor.cpp
	$(CC) -c Actor.cpp

TelnetSocket.o: TelnetSocket.cpp
	$(CC) $(LOGGING) -c TelnetSocket.cpp 

TelnetActor.o: TelnetActor.cpp
	$(CC) $(LOGGING) -c TelnetActor.cpp 

TelnetActorTest.o: TelnetActorTest.cpp
	$(CC) $(LOGGING) -c TelnetActorTest.cpp $(BOOST_THREAD)

Telnet.o: Telnet.cpp
	$(CC) -c Telnet.cpp $(BOOST_SYSTEM) $(LOGGING) 

Sensor.o: Sensor.cpp
	$(CC) -c Sensor.cpp

Controller.o: Controller.cpp
	$(CC) -c Controller.cpp

Configuration.o: Configuration.cpp
	$(CC) -c Configuration.cpp $(BOOST_SYSTEM)

ConfigurationTest.o: ConfigurationTest.cpp
	$(CC) -c ConfigurationTest.cpp $(LOGGING)

Notification.o: Notification.cpp
	$(CC) $(LOGGING) -c Notification.cpp

NotificationTest.o: NotificationTest.cpp
	$(CC) $(LOGGING) -c NotificationTest.cpp

WatchDescriptorEntry.o: WatchDescriptorEntry.cpp
	$(CC) $(LOGGING) -c WatchDescriptorEntry.cpp

WatchDescriptorEntryTest.o: WatchDescriptorEntryTest.cpp
	$(CC) -c WatchDescriptorEntryTest.cpp

main: main.cpp $(OBJECTS)
	$(CC) main.cpp -o $(EXE) $(OBJECTS) $(BOOST_THREAD) $(LOGGING) $(BOOST_SYSTEM) $(PTHREAD)

.cpp.obj:
	$(CC) -c $<

clean:
	rm -f *.o *.log
	rm -f $(EXE)
	rm -f $(TEST_EXE)

