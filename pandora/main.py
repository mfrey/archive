#!/usr/bin/env python
# Author: Michael Frey

import os
import csv
import sys
import copy
import pylab
import random
import logging
import argparse
import traceback
import itertools
import networkx as nx

from general import settings as cfg
from general import dotfilereader as dfr
from trace import logfilegenerator as log
from network import packet as pck
from network import wirelessnetwork as wifi

#logging.basicConfig(filename='example.log',level=logging.DEBUG)

class Helper:
  def cumulated_sum(self,iterable):
    iterable= iter(iterable)
    s = iterable.next()
    yield s
    for c in iterable:
      s = s + c
      yield s

class EnergyException(Exception):
  def __init__(self, value):
    self.value = value

  def __str__(self):
    return repr(self.value)

class EnergyAwareAntAlgorithm:
  def __init__(self):
    self.network = wifi.WirelessNetwork()
    self.settings = cfg.Settings()
    self.helper = Helper()
    self.routes = {}
    self.routing_mode = ['weighted','random','best'] 
    self.routing_mode_set = 0
    self.evaporation_mode = ['exponential','cubic'] 
    self.evaporation_mode_set = 0

  def add_node_to_route_statistics(self, packet, node):
    if packet in self.routes:
      self.routes[packet].append(node)
    else:
      self.routes[packet] = [packet, node]

  def send(self, packet, packet_trace, sender, destination):
      logging.debug('send called for sender ' + str(sender) + " and destination " + str(destination))

      if sender != destination:
        if not self.network.is_active(sender):
    #      self.network.dumpRoutingTable()
    #    print sender, self.network.network.node[sender]['energy']
          raise EnergyException('no remaining energy in sender')
        # pick the next node
        node = self.get_next_node(packet, sender, destination)
        # add node to packet_trace 
        packet_trace.visited_nodes.append(sender)
        # save current xii value of the node
        packet_trace.xii_list.append(self.network.get_energy_level(sender))

        # check if the next node is a valid node
        if node != -1:
          self.add_node_to_route_statistics(packet, sender)
          # calculate the transmission costs
          self.updateTransmissionCosts(packet, sender, destination)
          # update the pheromone values
          self.updatePheromoneValues(packet, sender, node, destination)
          # forward packet
          self.send(packet, packet_trace, node, destination)
        else:
          raise EnergyException('no remaining energy in next node')
      else:
        # add node to packet_trace 
        packet_trace.visited_nodes.append(sender)
        # set hop count
        packet_trace.hop_count = len(packet_trace.visited_nodes) - 2

        self.add_node_to_route_statistics(packet, destination)

        for node in self.network.network.nodes():
          self.network.log_energy_consumption(packet, node, self.network.network.node[node]['energy'])

  def get_next_node(self, packet, sender, destination):
    logging.debug('get next node for node ' + str(sender) + ' towards destination node ' +  str(destination))
    # get the neighbor nodes towards a destination
    nodes = self.network.getNeighborsTowardsDestination(packet, sender, destination)
    logging.debug('neighbors of node ' + str(sender) + " towards destination are " + str(nodes))
    # count how many of the neighbor nodes are in active 
    inactive  = 0

    for n in nodes:
      # check if neighbor is still active
      if self.network.get_energy_status(n):
        break
      else:
        inactive = inactive + 1
	
    if inactive == len(nodes):
      logging.debug('no active neighbor nodes at node ' + str(sender))
      # no active nodes anymore 
      return -1
	
    if len(nodes) == 1:
		return nodes[0]
	
    if len(nodes) > 0:
      # weighted mode, default
      if self.routing_mode[self.routing_mode_set] == 'weighted':
        distribution = self.forwarding(packet, sender, nodes, destination)
        node = nodes[self.getPath(distribution)]
      # simply pick a random route
      #elif self.routing_mode[self.routing_mode_set] == 'random':
      #
      # pick the best route (highest pheromone)
      else:
        node = self.network.get_highest_phi(packet, source, nodes, destination)
      return node

  def forwarding(self, packet, sender, nodes, destination):
    # initialize the distribution
    p = [0] * len(nodes)	 
    # the denominator of the probability function
    p_sum = 0
    
    #print "******* ******"
    #print "len(nodes):", len(nodes)
    
    # calculate the sum
    for node in nodes:
      phi   = self.network.get_phi(packet, sender, node, destination)
      #print "======> phi:", phi
      logging.debug('phi for edge(' + str(sender) + ', ' + str(node) + ') is ' + str(phi))
      xii   = self.network.get_energy_level(node)
      logging.debug('energy level of node ' + str(node) + ' is ' + str(xii))
      #print "======> xii:", xii
      # todo: check if this is really correct
      p_sum = p_sum + pow(phi, self.settings.alpha) + pow(xii, self.settings.beta)
      #p_sum = p_sum + pow(phi, self.settings.alpha) 
      
    #print "=======> p_sum:", p_sum

    logging.debug('p_sum is ' + str(p_sum))
    # calculate the probablity
    for index, value in enumerate(p):
      phi   = self.network.get_phi(packet, sender, nodes[index], destination)
      xii   = self.network.get_energy_level(nodes[index])
      p[index] = (pow(phi, self.settings.alpha) + pow(xii, self.settings.beta))/p_sum
      #p[index] = (pow(phi, self.settings.alpha))/p_sum

    # return the distribution 
    return p

  def getPath(self,distribution):
    if len(distribution) == 1:
      return 0
    else:
      # compute the cumulated density
      cumulatedDensity = list(self.helper.cumulated_sum(distribution))
      logging.debug('cumulated density is  ' + str(cumulatedDensity))
      # get a random number
      q = random.uniform(0,1)
      # find in which section of the cumulated density it's located
      path = 0
      
      while q > cumulatedDensity[path]:
      	path += 1
      
#       while path < (len(distribution) - 1):
#         if(q < cumulatedDensity[path]):
#           break;
#         else:
# 	      path = path + 1
      # return the path
      logging.debug('path is  ' + str(path))
      return path;

  def updateTransmissionCosts(self, packet, sender, destination):
    current_energy_level = self.network.get_energy_level(sender)
    # update the current energy level of the sender 
    new_energy_level = current_energy_level - self.settings.send

    if new_energy_level < 0:
      new_energy_level = 0
      self.network.set_energy_status(sender, False)
      # update phi value
      self.network.reset_phi(packet, sender)

    # set the new energy level
    self.network.set_energy_level(packet, sender, new_energy_level)
    # todo

    # get neighbors of the sender
    neighbors = self.network.get_neighbors(sender)
    # update the energy level for the node which receives the data (receiver + neighbors(sender))
    for node in neighbors:
      current_energy_level = self.network.get_energy_level(node)
      # update the current energy level of the receiving node 
      new_energy_level = current_energy_level - self.settings.recv
      if new_energy_level < 0:
        new_energy_level = 0
        self.network.set_energy_status(node, False)
        self.network.reset_phi(packet, node)
      # set the new energy level
      self.network.set_energy_level(packet, node, new_energy_level)

  def updatePheromoneValues(self, packet, sender, nextHop, destination):
    # iterate over each nodes routing table
    for node in self.network.network.nodes():
      if node != destination:
        # iterate over the routing table entries of the node
        for entry in self.network.network.node[node]['routing table']._table[packet].keys():
          phi = self.network.network.node[node]['routing table']._table[packet][entry].phi
          if entry[0] != sender:
            # decrease pheromone value
            phi = self.decrease_phi(phi)
          else:
            # get the next hop
            node_j = self.network.network.node[node]['routing table']._table[packet][entry].node_j
            # get the destination of the routing table entry
            dst = self.network.network.node[node]['routing table']._table[packet][entry].destination
            # if both matches, increase the edge, else decrease
            if dst == destination and node_j == nextHop: 
              phi = self.increasePheromoneValue(phi)
              #result = "##### updatePheromoneValues: increase value of edge("+ str(entry[0]) + "," + str(entry[1]) + ") = " + str(phi)
              #print result
            else:
              #result = "##### updatePheromoneValues: decrease value of edge("+ str(entry[0]) + "," + str(entry[1]) + ") = " + str(phi)
              # decrease pheromone value
              phi = self.decrease_phi(phi)
              #print result
          # update the routing table
          self.network.set_phi(packet, entry[0], entry[1], entry[2], phi)
#          self.network.network.node[node]['routing table']._table[packet][entry].phi = phi          
          # if phi is below a threshould
          #if phi < 0.009 and entry[1] != destination:          
          #  self.network.network.node[entry[1]]['active'] = False
          #result = "##### updatePheromoneValues: edge("+ str(entry[0]) + "," + str(entry[1]) + ") = " + str(phi)
          #print result

  def increasePheromoneValue(self, phi):
    return (phi + self.settings.delta_phi)

  def decrease_phi(self, phi):
    if self.evaporation_mode[self.evaporation_mode_set] is "exponential":
      phi = self.exponential_phi_decrease(phi)
    else:
      phi = self.cubic_phi_decrease(phi)
    return phi

  def exponential_phi_decrease(self, phi):
    return (phi * (1 - self.settings.q))

  def cubic_phi_decrease(self, phi):
    a = 1 - (2 * phi) 

    if a > 0:
      t = 0.5 * ((abs(a)**(1/self.settings.plateau)) + 1)
    else:
      t = 0.5 * (1 - (abs(a)**(1/self.settings.plateau)))

    m = t + (self.settings.reduction * self.settings.slow)

    r = 0.5 * ((((2 * m) - 1)**self.settings.plateau) + 1)

    if(r < 0):
      return 0
    elif(r > 1):
      return 1
    else:
      return r

  def writeLogFile(self, directory, file_name, content):
    current_path = os.getcwd()
    log_file = csv.writer(open(current_path + "/" + directory + "/" + file_name, "w"), delimiter=',')
    for entry in content:
      log_file.writerow(entry)

  def writeEnergyConsumptionTrace(self, directory, file_name):
    content = self.network.energy_consumption
    self.writeLogFile(directory, file_name, content)

  def writeRoutingTableTrace(self, directory, file_name):
    content = self.network.getRoutingTableTrace()
    self.writeLogFile(directory, file_name, content)

  def writeRoutingDecisionTrace(self, directory, file_name):
    current_path = os.getcwd()
    log_file = csv.writer(open(current_path + "/" + directory + "/" + file_name, "w"), delimiter=',')
    for key, value in self.routes.items():
      log_file.writerow(value)

def main():
  parser = argparse.ArgumentParser(description='evaluation script for the energy aware ant routing algorithm')
  parser.add_argument('-p', dest='packets', type=int, default=10, action='store', help='number of packets to send')
  parser.add_argument('-s', dest='send', type=float, default=0.02, action='store', help='energy consumed per send operation')
  parser.add_argument('-r', dest='receive', type=float, default=0.01, action='store', help='energy consumed per receive operation')
  parser.add_argument('-e', dest='energy', type=float, default=1.0, action='store', help='initial energy value')
  parser.add_argument('-q', dest='q', type=float, default=0.2, action='store', help='q parameter for evaporation process')
  parser.add_argument('-d', dest='delta', type=float, default=0.1, action='store', help='delta phi parameter for pheromone increase')
  parser.add_argument('-a', dest='alpha', type=float, default=1.0, action='store', help='weight alpha for transmission probablity')
  parser.add_argument('-b', dest='beta', type=float, default=1.0, action='store', help='weight beta for transmission probablity')
  parser.add_argument('-P', dest='phi', type=float, default=1.0, action='store', help='initial phi value')
  parser.add_argument('-i', dest='dot_file', type=str, default="", action='store', help='dot file')

  args = parser.parse_args()

  # create a settings object
  settings = cfg.Settings()

  # set the number of packets
  packets = args.packets 
  logging.debug('set number of packets to ' + str(packets))
  # set the alpha weight
  settings.alpha = args.alpha
  logging.debug('set alpha to ' + str(settings.alpha))
  # set the alpha weight 
  settings.beta = args.beta
  logging.debug('set beta to ' + str(settings.beta))
  # set the value for the energy which is consumed per receive operation
  settings.recv = args.receive
  logging.debug('set energy consumption for receive operation to ' + str(settings.recv))
  # set the value for the energy which is consumed per send operation
  settings.send = args.send
  logging.debug('set energy consumption for send operation to ' + str(settings.send))
  # set the q parameter (evaporation process)
  settings.q = args.q
  logging.debug('set q paramater of pheromone evaporation to ' + str(settings.q))
  # set the delta phi parameter 
  settings.delta_phi = args.delta
  logging.debug('set delta phi paramater of pheromone increase to ' + str(settings.delta_phi))
  # set the initial phi parameter (node)
  settings.phi = args.phi
  logging.debug('set initial phi to ' + str(settings.phi))
  # set the initial energy parameter (node)
  settings.xii = args.energy
  logging.debug('set initial energy to ' + str(settings.xii))
  # set the dot file 
  settings.dot_file = args.dot_file
  if settings.dot_file != "":
    logging.debug('will load dot file' + str(settings.dot_file))

  network = nx.Graph()

  if settings.dot_file == "":
    network.add_nodes_from([1,4]);
#  network.add_nodes_from([1,8]);
  #network.add_nodes_from([1,11]);
  #network.add_edges_from([(1,2),(1,3),(1,4),(2,5),(5,7),(7,8),(3,6),(6,8),(4,8)])
    network.add_edges_from([(1,2),(1,3),(2,4),(3,4)])
  #network.add_edges_from([(1,2),(1,3),(1,4),(2,5),(5,8),(8,10),(10,11),(3,6),(6,9),(9,11),(4,7),(7,11)])
  else:
    dot_file_reader = dfr.DotFileReader()
    network = dot_file_reader.read_dot_file(settings.dot_file)

  w = wifi.WirelessNetwork()
  w.network = network
  # set the settings
  w.settings = settings
  # set up the initial routing table
  w.setInitialRoutingTable()

#  w.setupRoutingTable(1,8)
#  w.setupRoutingTableShortestPaths(1,8)
  w.setInitialEnergyLevel(settings.xii)
  #w.initialize_route_discovery(1,8)
  w.initialize_route_discovery(1,4)

#w.network.node[2]['energy'] = 5.0
#w.network.node[3]['energy'] = 5.0
#w.network.node[4]['energy'] = 5.0

#  w.set_phi(0, 1, 3, 8, settings.phi * 2)
#  w.set_phi(0, 3, 6, 8, settings.phi * 2)
#  w.set_phi(0, 6, 8, 8, settings.phi * 2)

  w.position = nx.spring_layout(w.network)
  #w.draw()

  algorithm = EnergyAwareAntAlgorithm()
  algorithm.settings = settings
  algorithm.network = w

  packet_trace_container = {}

  for packet in range(1,packets):
    # duplicate the initial entry
    for n in algorithm.network.network.nodes():
      algorithm.network.network.node[n]['routing table'].duplicate_entries(packet)
    try:
        packet_trace = pck.Packet()
        packet_trace_container[packet] = packet_trace
        packet_trace.src = 1
        packet_trace.dst = 4 
        # we geht the current energy level of the network (initial energy level)
        packet_trace.network_initial_xii = algorithm.network.get_network_energy()
        algorithm.send(packet, packet_trace, 1, 4)
        # we geht the current energy level of the network (energy level after transmissions)
        packet_trace.network_final_xii = algorithm.network.get_network_energy()
        # compute the tranmission costs
        packet_trace.set_transmission_costs() 
        #
        packet_trace.xii_dict = algorithm.network.get_network_energy_as_dict()
    except EnergyException, err:
      print err
      del packet_trace_container[packet]
      break

  generator = log.LogFileGenerator()
  generator.packet_trace = packet_trace_container
  generator.generate_routing_decision_trace('test_routingdecision_trace.csv')
  generator.generate_paths()
  generator.network = w
  generator.settings = settings
  generator.position = nx.spring_layout(network)
  generator.write_routes_log_file('routes.csv')
  generator.write_energy_log_file('energy.csv')
  generator.write_energy_per_route_log_file('energy_total.csv')
  generator.write_routing_decision('routing_decision_trace.csv')
  generator.write_last_sucessful_packet('last_packet.csv')
  #generator.animate()
#  generator.write_route_pheromone_log_file('route_pheromone_log.csv')
  generator.generate_active_path_phi('route_pheromone_log.csv')
  generator.write_route_trace_mg('route_trace_mg.csv')
  #generator.write_route_pheromone_log_file('route_pheromone_log.csv')

#algorithm.writeEnergyConsumptionTrace('energy.csv')
  algorithm.writeRoutingTableTrace('routingtable_trace.csv')
  algorithm.writeRoutingDecisionTrace('routingdecision_trace.csv')

if __name__ == "__main__":
  main()
