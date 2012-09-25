#!/usr/bin/env python

import pp

class ExperimentSettings:
  def initialize(self):
    self.name = ""
    self.initial_pheromone = 0.0
    self.initial_energy = 0.0
    self.alpha_list = []
    self.beta_list = []
    self.delta_phi = 0.0
    self.q = 0.0
    self.packets = 1000

  def __str__(self):
    self.name 

class Experiment:
  def initialize(self):
    self.repetitions = 30
#    self.algorithm = EnergyAwareAntAlgorithm()

  def read_configuration(self, file_name)
    configuration_file = open(file_name, "r")
    for line in configuration_file:
      if line.startswith(";"):
      
    configuration_file.close()


def main():
  parser = argparse.ArgumentParser(description='evaluation script for the energy aware ant routing algorithm')
  parser.add_argument('-c', dest='configuration', type=str, default="", action='store', help='configuration settings')

#  job_server = pp.Server() 


