#!/usr/bin/env python

import ConfigParser

class ConfigurationFile:
  def initialize(self):
    self.filename = ""

  def read(self, filename):
    configuration = ConfigParser.ConfigParser()
    # read the configuration file
    configuration.read(filename)
    # 
    alpha = configuration.get('Experiment','alpha')
    # 
    beta = configuration.get('Experiment','alpha')

