#!/usr/bin/env python

import re
import ConfigParser

class ExperimentSettings:
  def initialize(self):
	# the name of the experiments
    self.name = ""
	# the initial pheromone value per edge
    self.phi = 0.0
	# the initial energy value of a node
    self.xii = 0.0
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
    # 
    self.topology = ""
    # 
    self.src = -1
    # 
    self.dst = -1
    # 
    self.repetitions = 30
    # 
    self.send = 0.01
    #
    self.recv = 0.01
    #
    self.evaporation_mode = 0

  def __str__(self):
    self.name 

class ConfigurationFile:
  def initialize(self):
    self.filename = ""

  def get_evaporation_mode(self, mode):
    if mode == "cubic":
      return 1
    return 0

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
    packets = int(configuration.get('Experiment','packets'))
	# read the initial energy parameter and cast it to a float value
    xii = float(configuration.get('Experiment','initial_energy'))
	# read the initial pheromone parameter and cast it to a float value
    phi = float(configuration.get('Experiment','initial_pheromone'))
	# read the energy costs per receive parameter and cast it to a float value
    recv = float(configuration.get('Experiment','energy_recv'))
	# read the energy costs per send parameter and cast it to a float value
    send = float(configuration.get('Experiment','energy_send'))
    # read the location of the topology file
    topology = configuration.get('Experiment','topology')
    # read the location of the topology file
    if configuration.get('Experiment', 'node_selection') == "random":
      src = dst = -1
    else:
      src = int(configuration.get('Experiment','source'))
      # read the location of the topology file
      dst = int(configuration.get('Experiment','destination'))
    # read the location of the topology file
    repetitions = int(configuration.get('Experiment','repetitions'))
    # read the evporation mode
    evaporation_mode = self.get_evaporation_mode(re.sub(r'\s', '',configuration.get('Experiment','evaporation_mode')))
    print evaporation_mode
	# build up the experiment settings
    settings = ExperimentSettings()
    settings.alpha_list = alpha
    settings.beta_list = beta
    settings.delta_phi = delta_phi
    settings.q = q
    settings.packets = packets
    settings.phi = phi
    settings.xii = xii
    settings.topology = topology
    settings.src = src
    settings.dst = dst
    settings.repetitions = repetitions
    settings.send = send
    settings.recv = recv
    settings.evaporation_mode = evaporation_mode
	# return the results
    return settings
