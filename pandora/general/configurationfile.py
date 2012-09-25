#!/usr/bin/env python

import re
import ConfigParser

class ExperimentSettings:
  def initialize(self):
	# the name of the experiments
    self.name = ""
	# the initial pheromone value per edge
    self.initial_pheromone = 0.0
	# the initial energy value of a node
    self.initial_energy = 0.0
	# a list of alpha weights (transmission probability)
    self.alpha_list = []
	# a list of beta weights (transmission probability)
    self.beta_list = []
	# the variable controls the pheromone increase process
    self.delta_phi = 0.0
	# the variable controls the pheromone evaporation process
    self.q = 0.0
	# the maximum number of packets which should be sent
    self.packets = 1000

  def __str__(self):
    self.name 

class ConfigurationFile:
  def initialize(self):
    self.filename = ""

  def read(self, filename):
    configuration = ConfigParser.ConfigParser()
    # read the configuration file
    configuration.read(filename)
    # read the alpha parameter list and remove the whitespaces from the string
    alpha = re.sub(r'\s', '', configuration.get('Experiment','alpha'))
	# split the entries and cast to entries to float
    alpha = [float(entry) for entry in alpha.split(',')]
    # read the beta parameter list and remove the whitespaces from the string
    beta = re.sub(r'\s', '', configuration.get('Experiment','beta'))
	# split the entries and cast to entries to float
    beta = [float(entry) for entry in beta.split(',')]
	# read the delta phi parameter and cast it to a float value
    delta_phi = float(configuration.get('Experiment','delta_phi'))
	# read the q parameter and cast it to a float value
    q = float(configuration.get('Experiment','q'))
	# read the packet parameter and cast it to a int value
    packets = float(configuration.get('Experiment','packets'))
	# read the packet parameter and cast it to a int value
    xii = float(configuration.get('Experiment','initial_energy'))
	# read the packet parameter and cast it to a int value
    phi = float(configuration.get('Experiment','initial_pheromone'))
	# build up the experiment settings
    settings = ExperimentSettings()
    settings.alpha_list = alpha
    settings.beta_list = beta
    settings.delta_phi = delta_phi
    settings.q = q
    settings.packets = packets
    settings.initial_energy = xii
    settings.initial_pheromone = phi
	# return the results
    return settings
