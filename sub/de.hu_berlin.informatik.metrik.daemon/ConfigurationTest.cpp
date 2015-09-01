#include "include/ConfigurationTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(test::ConfigurationTest);

using namespace log4cxx;

LoggerPtr test::ConfigurationTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.configurationtest"));

void test::ConfigurationTest::setUp(void){
  c = new Configuration();
  d = new Configuration(std::string("settings/test/test_1.conf"));
}

void test::ConfigurationTest::tearDown(void){
  delete c; 
  delete d;
}

void test::ConfigurationTest::testReadConfigurationFile(void){
  // A configuration file for testing the readConfigurationFile() method of the Configuration class 
  //std::string configurationFile("test_configurationfile.conf"){}
  std::string configurationFile("settings/test/test_1.conf");

  LOG4CXX_TRACE(mLogger, "attempt to set location of configuration file to " << configurationFile);
  c->setConfigurationFile(configurationFile);

  LOG4CXX_TRACE(mLogger, "attempt to read configuration file");
  c->readConfigurationFile();
}

void test::ConfigurationTest::testGetLoggingFile(){
  std::string loggingFile = d->getLoggingFile();
  LOG4CXX_TRACE(mLogger, "logging file seems to be " << loggingFile);
  CPPUNIT_ASSERT(loggingFile.compare("settings/logging/logging_cppunit.properties") == 0);
}

void test::ConfigurationTest::testGetConfigurationFile(){
  std::string configurationFile = d->getConfigurationFile();
  LOG4CXX_TRACE(mLogger, "configuration file seems to be " << configurationFile);
  CPPUNIT_ASSERT(configurationFile.compare("settings/test/test_1.conf") == 0);
}

void test::ConfigurationTest::testGetTelnetActorHost(){
  std::string host = d->getTelnetActorHost();
  LOG4CXX_TRACE(mLogger, "telnet host seems to be " << host);
  CPPUNIT_ASSERT(host.compare("localhost") == 0);
}

void test::ConfigurationTest::testGetTelnetActorPort(){
  std::string port = d->getTelnetActorPort();
  LOG4CXX_TRACE(mLogger, "telnet port seems to be " << port);
  CPPUNIT_ASSERT(port.compare("23") == 0);
}

void test::ConfigurationTest::testGetFilesToWatch(){
  std::vector<std::string> files = d->getFilesToWatch();
  LOG4CXX_TRACE(mLogger, "list with files to watch seems to be of size  " << files.size());
  CPPUNIT_ASSERT(files.size() == 4);
}

void test::ConfigurationTest::testGetDirectoriesToWatch(){
  std::vector<std::string> directories = d->getDirectoriesToWatch();
  LOG4CXX_TRACE(mLogger, "list with directories to watch seems to be of size  " << directories.size());
  CPPUNIT_ASSERT(directories.size() == 3);
}

void test::ConfigurationTest::parseOptions(){}

void test::ConfigurationTest::testSetConfigurationFile(){}

void test::ConfigurationTest::enableLogging(){}

void test::ConfigurationTest::testGetWatchDescriptorEntries(){}

void test::ConfigurationTest::testGetEntries(){}
