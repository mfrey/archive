#include "include/ConfigurationTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(test::ConfigurationTest);

using namespace log4cxx;

LoggerPtr test::ConfigurationTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.configurationtest"));

void test::ConfigurationTest::setUp(void){
  c = new Configuration();
}

void test::ConfigurationTest::tearDown(void){
  delete c;
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

void test::ConfigurationTest::testGetLoggingFile(){}
void test::ConfigurationTest::testGetConfigurationFile(){}
void test::ConfigurationTest::testGetTelnetActorHost(){}
void test::ConfigurationTest::testGetTelnetActorPort(){}
void test::ConfigurationTest::testGetFilesToWatch(){}
void test::ConfigurationTest::testGetDirectoriesToWatch(){}
void test::ConfigurationTest::parseOptions(){}
void test::ConfigurationTest::testSetConfigurationFile(){}
void test::ConfigurationTest::enableLogging(){}
void test::ConfigurationTest::testGetWatchDescriptorEntries(){}
void test::ConfigurationTest::testGetEntries(){}
