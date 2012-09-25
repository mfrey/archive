#!/usr/bin/env python

import pp
import argparse

from general import configurationfile as cfg

class Experiment:
  def initialize(self):
	self.settings = cfg.ExperimentSettings()
#    self.algorithm = EnergyAwareAntAlgorithm()

  def read_configuration(self, file_name):
    configuration = cfg.ConfigurationFile()
    self.settings = configuration.read(file_name)



def main():
  parser = argparse.ArgumentParser(description='evaluation script for the energy aware ant routing algorithm')
  parser.add_argument('-c', dest='configuration_file', type=str, default="", action='store', help='configuration settings')

  arguments = parser.parse_args()

  experiment = Experiment()
  experiment.read_configuration(arguments.configuration_file)

#  job_server = pp.Server() 

if __name__ == "__main__":
  main()

