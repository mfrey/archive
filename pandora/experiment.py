#!/usr/bin/env python

import os
import logging
import argparse
import multiprocessing

import main as m
import networkx as nx

from general import dotfilereader as dfr
from general import settings as expcfg
from network import packet as pck
from trace import logfilegenerator as log
from network import wirelessnetwork as wifi
from general import configurationfile as cfg

logging.basicConfig(filename='experiment.log',level=logging.DEBUG)

class Experiment:
  def __init__(self):
    self.settings = expcfg.Settings()
    self.network = wifi.WirelessNetwork()
    self.algorithm = m.EnergyAwareAntAlgorithm()
    self.packet_trace_container = {}
    self.log_dir = ""

  def create_log_directory(self, directory):
    if not os.path.exists(directory):
      os.makedirs(directory)

  def setup_experiment(self):
    # store the settings
    self.algorithm.settings = self.settings
    # set the network
    self.algorithm.network = self.network
    # set the log file generator
    self.generator = log.LogFileGenerator()

  def run_experiment(self, packets, source, destination):
    # set the evaporation mode (TODO: find a better place to set the evaporation mode)
    self.algorithm.evaporation_mode_set = self.algorithm.settings.evaporation_mode
    # route discovery
    self.network.initialize_route_discovery(source, destination)
    # try to send all packets
    for packet in range(1, packets):
      # duplicate the initial entry
      for n in self.algorithm.network.network.nodes():
        self.algorithm.network.network.node[n]['routing table'].duplicate_entries(packet)
      try:
        packet_trace = pck.Packet()
        self.packet_trace_container[packet] = packet_trace
        packet_trace.src = source 
        packet_trace.dst = destination
        # we geht the current energy level of the network (initial energy level)
        packet_trace.network_initial_xii = self.algorithm.network.get_network_energy()
        self.algorithm.send(packet, packet_trace, source, destination)
        # we geht the current energy level of the network (energy level after transmissions)
        packet_trace.network_final_xii = self.algorithm.network.get_network_energy()
        # compute the tranmission costs
        packet_trace.set_transmission_costs() 
        #
        packet_trace.xii_dict = self.algorithm.network.get_network_energy_as_dict()
      except m.EnergyException, err:
        print err
        del self.packet_trace_container[packet]
        break

  def setup_network(self, topology_file):
    dot_file_reader = dfr.DotFileReader()
    # read the configuration file 
    dot_network = dot_file_reader.read_dot_file(topology_file)
    # pass the network configuration to the wireless network
    self.network.network = dot_network
    # set the settings
    self.network.settings = self.settings
    # set up the network
    self.network.setup()
    # set the layout for drawing the network
    self.network.position = nx.spring_layout(self.network.network)

  def generate_log_files(self):
    self.generator.packet_trace = self.packet_trace_container
    self.generator.generate_routing_decision_trace(self.log_dir, 'test_routingdecision_trace.csv')
    self.generator.generate_paths()
    self.generator.network = self.network
    self.generator.settings = self.settings
    # throws an exception, why?
    #self.generator.position = nx.spring_layout(self.network)
    self.generator.write_routes_log_file(self.log_dir, 'routes.csv')
    self.generator.write_energy_log_file(self.log_dir, 'energy.csv')
    # DEACTIVATED:
    #self.generator.write_energy_per_route_log_file(self.log_dir, 'energy_total.csv')
    self.generator.write_routing_decision(self.log_dir, 'routing_decision_trace.csv')
    # DEACTIVATED:
    #self.generator.write_last_sucessful_packet(self.log_dir, 'last_packet.csv')
    #generator.animate()
#  generator.write_route_pheromone_log_file('route_pheromone_log.csv')
    self.generator.generate_active_path_phi(self.log_dir, 'route_pheromone_log.csv')
    # DEACTIVATED:
    #self.generator.write_route_trace_mg(self.log_dir, 'route_trace_mg.csv')

    #generator.write_route_pheromone_log_file('route_pheromone_log.csv')

    #algorithm.writeEnergyConsumptionTrace('energy.csv')

    self.algorithm.writeRoutingTableTrace(self.log_dir, 'routingtable_trace.csv')

    # DEACTIVATED:
#    self.algorithm.writeRoutingDecisionTrace(self.log_dir, 'routingdecision_trace.csv')


def worker(num):
  print 'Worker:', num
  #parser = argparse.ArgumentParser(description='evaluation script for the energy aware ant routing algorithm')
  #parser.add_argument('-c', dest='configuration_file', type=str, default="", action='store', help='configuration settings')

  #arguments = parser.parse_args()
  arguments_configuration_file = "settings-" + str(num) + ".ini"

  if arguments_configuration_file != "":
    # automatically detect number of possible workers
    configuration = cfg.ConfigurationFile()
    # read the configuration file
    configuration_settings = configuration.read(arguments_configuration_file)
    # set the repetitions
    repetitions = configuration_settings.repetitions + 1
    for alpha in configuration_settings.alpha_list:
      for beta in configuration_settings.beta_list:
        for repetition in range(1, repetitions):
          # prepare the experiment
          settings = expcfg.Settings()
          # set the number of packets
          packets = configuration_settings.packets 
          logging.debug('set maximum packets to ' + str(packets))
          # set the alpha weight
          settings.alpha = alpha
          logging.debug('set alpha to ' + str(settings.alpha))
          # set the alpha weight 
          settings.beta = beta
          logging.debug('set beta to ' + str(settings.beta))
          # set the value for the energy which is consumed per receive operation
          settings.recv = configuration_settings.recv
          logging.debug('set energy costs for receiving operations to ' + str(settings.recv))
          # set the value for the energy which is consumed per send operation
          settings.send = configuration_settings.send
          logging.debug('set energy costs for send operations to ' + str(settings.send))
          # set the q parameter (evaporation process)
          settings.q = configuration_settings.q
          logging.debug('set q parameter for the evaporation process to ' + str(settings.q))
          # set the delta phi parameter 
          settings.delta_phi = configuration_settings.delta_phi
          logging.debug('set delta phi parameter for pheromone increase to ' + str(settings.delta_phi))
          # set the initial phi parameter (node)
          settings.phi = configuration_settings.phi
          logging.debug('set initial phi parameter to ' + str(settings.phi))
          # set the initial energy parameter (node)
          settings.xii = configuration_settings.xii
          logging.debug('set initial energy value of the nodes to ' + str(settings.xii))
          # set the dot file 
          settings.dot_file = configuration_settings.topology
          # set the evaporation mode
          settings.evaporation_mode = configuration_settings.evaporation_mode

          experiment = Experiment()
          experiment.settings = settings
          directory = 'experiment-' + str(num) + "-" + str(configuration_settings.packets) + '-' + str(alpha) + '-' + str(beta) 
          logging.debug('create directory ' + str(directory))
          # create log directory
          experiment.create_log_directory(directory)
          # create the network and set it up
          experiment.setup_network(configuration_settings.topology)
          # setup the experiment
          experiment.setup_experiment()
          # run the experiment
          experiment.run_experiment(configuration_settings.packets, configuration_settings.src, configuration_settings.dst)
          # 
          directory = 'experiment-' + str(num) + "-" + str(configuration_settings.packets) + '-' + str(alpha) + '-' + str(beta) + '/' + str(repetition) 
          # create log directory
          experiment.create_log_directory(directory)
          experiment.log_dir = directory
          experiment.generate_log_files()


if __name__ == "__main__":
  jobs = []
  for i in range(11):
    p = multiprocessing.Process(target=worker, args=(i,))
    jobs.append(p)
    p.start()
