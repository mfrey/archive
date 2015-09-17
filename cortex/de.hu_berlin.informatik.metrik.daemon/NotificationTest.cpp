#include "include/NotificationTest.h" 

CPPUNIT_TEST_SUITE_REGISTRATION(test::NotificationTest);

using namespace log4cxx;

LoggerPtr test::NotificationTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.notificationtest"));

void test::NotificationTest::setUp(){
  a = new Notification();
  // Initialize seed of random number generator
  srand(time(NULL));
}

void test::NotificationTest::tearDown(){
  delete a; delete b;
}

void test::NotificationTest::testAddRemove(){
  string fileName = createFileName();
  LOG4CXX_TRACE(mLogger, "create a file with file name: " << fileName);
  createFile(fileName.c_str());
  // Check if the object is initialized
  CPPUNIT_ASSERT(a != NULL);
  LOG4CXX_TRACE(mLogger, "will add a file with file name: " << fileName << " to list");
  // Add an element to the watch descriptor entry list hold by the notification class
  bool result = a->add(fileName.c_str(), IN_MODIFY);
  // Check if the operation was successful
  CPPUNIT_ASSERT(result == true);
  // Remove an element of the watch descriptor entry list hold by the notification class
  LOG4CXX_TRACE(mLogger, "will remove a file with file name: " << fileName << " to list");
  result =  a->remove(fileName.c_str());
  // Check if the operation was successful
  CPPUNIT_ASSERT(result == true);
}  

WatchDescriptorEntry test::NotificationTest::createWatchDescriptorEntry(const char* pFileName){
  LOG4CXX_TRACE(mLogger, "create a watch descriptor entry for file " << pFileName);
  CPPUNIT_ASSERT(a->getInotifyId() != -1);
  LOG4CXX_TRACE(mLogger, "add " << pFileName << " to watched files (via inotify)");
  int wd = inotify_add_watch(a->getInotifyId(), pFileName, IN_MODIFY);
  CPPUNIT_ASSERT(wd != -1);
  return WatchDescriptorEntry(wd, IN_MODIFY, pFileName);
}

void test::NotificationTest::createFile(const char* pFileName){
  ofstream file;
  file.open(pFileName);
  file.close();
}

string test::NotificationTest::createFileName(){
  // Create a string stream
  stringstream fileName;
  // Create file name
  fileName << "/tmp/CortexNotificationTest" << rand() << ".txt";
  // Set file name
  return fileName.str();
}

void test::NotificationTest::testAddRemoveWatchDescriptorEntry(){
  string fileName = createFileName();
  LOG4CXX_TRACE(mLogger, "create file " << fileName);
  createFile(fileName.c_str());
  // Check if the object is initialized
  CPPUNIT_ASSERT(a != NULL);
  // Check if inotify is initialized
  CPPUNIT_ASSERT(a->getInotifyId() != -1);
  // Create watch descriptor entry 
  LOG4CXX_TRACE(mLogger, "create watch descriptor entry for file " << fileName);
  WatchDescriptorEntry entry = createWatchDescriptorEntry(fileName.c_str());
  // Check if the object is initialized, TODO: Check if this is right

  // Add entry to the list
  LOG4CXX_TRACE(mLogger, "add watch descriptor entry for file " << fileName << " to list");
  bool result = a->addWatchDescriptorEntry(entry);
  CPPUNIT_ASSERT(result == true);
  // Remove entry from list
  result = a->remove(entry.getName().c_str());
  CPPUNIT_ASSERT(result == true);
}  

void test::NotificationTest::testAddRemoveWatchDescriptorEntries(){
  typedef std::tr1::unordered_map<const char*, WatchDescriptorEntry> map;
  map entries;
  // Create entries
  for(int i = 0; i < 2; i++){
     string fileName = createFileName();
     createFile(fileName.c_str());
     WatchDescriptorEntry entry = createWatchDescriptorEntry(fileName.c_str());
     std::pair<map::iterator, bool> r = entries.insert(std::pair<const char*,WatchDescriptorEntry>(entry.getName().c_str(), entry));
     CPPUNIT_ASSERT(r.second == true);
  }
  // Add entries to the list
  bool result = a->addWatchDescriptorEntries(entries);
  CPPUNIT_ASSERT(result == true);
  // Remove entries from the list
  for(map::iterator i = entries.begin(); i != entries.end(); ++i){
    result = a->remove(i->second.getName().c_str());
    CPPUNIT_ASSERT(result == true);
  }
}
