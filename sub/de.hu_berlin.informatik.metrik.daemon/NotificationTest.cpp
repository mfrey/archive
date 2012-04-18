#include "include/NotificationTest.h" 

CPPUNIT_TEST_SUITE_REGISTRATION(NotificationTest);

void NotificationTest::setUp(){
  a = new Notification();
  // Initialize seed of random number generator
  srand(time(NULL));
  // Create a string stream
  stringstream fileName;
  // Create file name
  fileName << "/tmp/CortexNotificationTest" << rand() << ".txt";
  // Set file name
  mFileName = fileName.str();
  // Create file
  mFile.open(mFileName.c_str());
}

void NotificationTest::tearDown(){
  delete a; delete b;
  mFile.close();
}

void NotificationTest::testAddRemove(){
  // Check if the object is initialized
  CPPUNIT_ASSERT(a != NULL);
  // Add an element to the watch descriptor entry list hold by the notification class
  bool result = a->add(mFileName.c_str(), IN_MODIFY);
  // Check if the operation was successful
  CPPUNIT_ASSERT(result == true);
  // Remove an element to the watch descriptor entry list hold by the notification class
  result =  a->remove(mFileName.c_str());
  // Check if the operation was successful
  CPPUNIT_ASSERT(result == true);
}  

WatchDescriptorEntry NotificationTest::createWatchDescriptorEntry(){
  CPPUNIT_ASSERT(a->getInotifyId() != -1);
  int wd = inotify_add_watch(a->getInotifyId(), mFileName.c_str(), IN_MODIFY);
  CPPUNIT_ASSERT(wd != -1);
  return WatchDescriptorEntry(wd, IN_MODIFY, mFileName.c_str());
}

void NotificationTest::testAddRemoveWatchDescriptorEntry(){
  // Check if the object is initialized
  CPPUNIT_ASSERT(a != NULL);
  // Check if inotify is initialized
  CPPUNIT_ASSERT(a->getInotifyId() != -1);
  // Create watch descriptor entry 
  WatchDescriptorEntry entry = createWatchDescriptorEntry();
  // Check if the object is initialized, TODO: Check if this is right
  CPPUNIT_ASSERT(&entry != NULL);
  // Add entry to the list
  bool result = a->addWatchDescriptorEntry(entry);
  CPPUNIT_ASSERT(result == true);
  // Remove entry from list
  result = a->remove(entry.getName().c_str());
  CPPUNIT_ASSERT(result == true);
}  

void NotificationTest::initialize(){

}

