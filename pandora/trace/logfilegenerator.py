#!/usr/bin/env python

import os
import csv
import sys
import copy
import pylab
import random
import logging
import argparse
import traceback

from operator import itemgetter

import os
import itertools
import networkx as nx

from general import settings as cfg
from network import packet as pck
from network import wirelessnetwork as wifi

module_logger = logging.getLogger(__name__)

class LogFileGenerator:
  def __init__(self):
    self.packet_trace = {}
    self.routes = {}
    self.network = wifi.WirelessNetwork()
    self.fu_green = "#99CC00"
    self.fu_blue = "#003366"
    self.fu_red = "#CC0000"
    self.fu_orange = "#FF9900"
    self.settings = cfg.Settings()
    self.logger = logging.getLogger(__name__)
    self.logger.setLevel(self.logger.DEBUG)

  def generate_routing_decision_trace(self, directory, file_name):
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for key in self.packet_trace:
      log_file.write(self.packet_trace[key].route_to_string() + "\n")
  
  def write_route_pheromone_log_file(self, file_name):
    attempts = 0
    log_file = open(file_name, "w")
    for packet in self.packet_trace:
      if attempts < 2:
        try:
          for key,value in self.packet_trace[packet].phi_dict.items():
            route_identifier = self.routes[key]
            route_as_list = list(key)
            hop_count = len(route_as_list) - 2
            row = str(packet) + ',' + str(route_identifier) + ',' + str(value) + ',' + str(hop_count) + '\n'
            log_file.write(row)
        except KeyError:
          # the path dictionary is probably not set up, generate paths
          self.generate_paths()
          # increment the attempt counter
          attempts += 1
    log_file.close()

  # aweful, please fix me
  # something like <packet id>, <path id>, <remaining energy>  
  def write_energy_log_file(self, directory, file_name):
    attempts = 0
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for packet in self.packet_trace:
      # generate a tuple which represents the key for the path list
      route = tuple(self.packet_trace[packet].visited_nodes)
      if attempts < 2:
        try:
          # look up the route id 
          route_identifier = self.routes[route]
          row = str(packet) + ',' + str(route_identifier) + ',' + str(self.packet_trace[packet].network_final_xii) + '\n'
          # build up the row to write 
          log_file.write(row)
        except KeyError:
          # the path dictionary is probably not set up, generate paths
          self.generate_paths()
          # increment the attempt counter
          attempts += 1
    log_file.close()

  # aweful, please fix me 
  # todo fix method name
  def generate_paths(self):
    temp_routes = {} 
    # get all paths
    for packet in self.packet_trace:
      route = tuple(self.packet_trace[packet].visited_nodes)
      if route not in temp_routes:
        temp_routes[route] = len(route)
    
    counter = 1

    for key, value in sorted(temp_routes.items(), key=itemgetter(1)):
      self.routes[key] = counter
      counter += 1

  def write_routes_log_file(self, directory, file_name):
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for route, identifier in self.routes.items():
      line = str(identifier) + ',' + ','.join(map(str, route)) + '\n'
      log_file.write(line)
    log_file.close()

  def compute_path_phi(self, packets, nodes, destination):
    phi = 0
    for node in len(nodes-1):
      phi += self.network.network[node]['routing table'].table[packet].get_phi(node, nodes[(node+1)], destination)
    return phi

  def write_energy_per_route_log_file(self, directory, file_name):
    attempts = 0
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for packet in self.packet_trace:
      if attempts < 2:
        try:
          for route in self.routes:
            # 'transform' the tuple to t a list
            route_as_list = list(route)
            hop_count = len(route_as_list) - 2
            # get the energy information of the path 
            energy = [self.packet_trace[packet].xii_dict[x] for x in route_as_list]
            # build up the sum 
            energy_sum = sum(energy)
            # look up the route id 
            route_identifier = self.routes[route]
            # 
            row = str(packet) + ',' + str(route_identifier) + ',' + str(energy_sum) + ',' + str(hop_count) + '\n'
            # build up the row to write 
            log_file.write(row)
        except KeyError:
          # the path dictionary is probably not set up, generate paths
          self.generate_paths()
    log_file.close()

  # todo 
  def animate(self):
    i = 0
    position = nx.spring_layout(self.network.network)
    for packet in self.packet_trace:
      for node in self.packet_trace[packet].visited_nodes:
        pylab.clf()
        pylab.subplot(111,aspect='equal')
        for n in self.network.network:
          # draw node with the respective size
          size = 1000 * self.packet_trace[packet].xii_dict[n]
          alpha = 1.0 * self.packet_trace[packet].xii_dict[n]
          nx.draw(self.network.network, position, with_labels=True, node_color=self.fu_green,node_size=size,alpha=alpha)

        size = 1000 * self.packet_trace[packet].xii_dict[node]
        nx.draw_networkx_nodes(self.network.network, position,nodelist=[node], node_color=self.fu_orange,size=size)
        pylab.draw()
        fname = '_tmp%03d.png'%i
        pylab.savefig(fname,dpi=100)
        i=i+1
    filenames="_tmp%03d.png"
    os.system("ffmpeg -r 3 -minrate 1024k -b 2024k -y -i %s path.mp4"%(filenames))

  def write_last_sucessful_packet(self, directory, file_name):
    last_packet = (len(self.packet_trace))
    try:
      route = self.routes[tuple(self.packet_trace[last_packet].visited_nodes)]
      row = str(self.settings.alpha) + "," + str(self.settings.beta) + "," + str(route) + "," + str(last_packet)
      cur_dir = os.getcwd()
      log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
      log_file.write(row)
    except KeyError:
      print "no such route"
    log_file.close()

  def write_route_trace_mg(self, directory, file_name):
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for packet in self.packet_trace:
      trace = self.packet_trace[packet].route_taken_mg()
      log_file.write(trace)
    log_file.close()

  def write_routing_decision(self, directory, file_name):
    attempts = 0
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for packet in self.packet_trace:
      if attempts < 2:
        try:
          route = tuple(self.packet_trace[packet].visited_nodes)
          if route in self.routes:
            # look up the route id 
            route_identifier = self.routes[route]
            row = str(packet) + ',' + str(route_identifier) + '\n'
            log_file.write(row)
        except KeyError:
          attempts += 1
          # the path dictionary is probably not set up, generate paths
          self.generate_paths()
    log_file.close()

  # the problem is we have multiple phi values per edge
  def generate_active_path_phi(self, directory, file_name):
    attempts = 0
    cur_dir = os.getcwd()
    log_file = open(cur_dir + "/" + directory + "/" + file_name, "w")
    for packet in self.packet_trace:
      if attempts < 2:
        try:
          for route in self.routes.keys():
            phi = 0.0
            for index, value in enumerate(route):
              if index < len(route)-1:
                current_hop, next_hop, destination = route[index], route[index+1], self.packet_trace[packet].dst
                phi += self.network.get_phi(packet, current_hop, next_hop, destination)
            # fix me
            hop_count = len(route) - 2
            row = str(packet) + ',' + str(self.routes[route]) +  ',' + str(phi) + ',' + str(hop_count) + '\n'
            log_file.write(row)
        except KeyError:
          attempts += 1
          # the path dictionary is probably not set up, generate paths
          self.generate_paths()
    log_file.close()

  def generate_statistics(self):
    #
    self.generate_routing_decision_trace('test_routingdecision_trace.csv')
    #
    self.generate_paths()
    #
    self.write_routes_log_file('routes.csv')
    #
    self.write_energy_log_file('energy.csv')
    #
    self.write_energy_per_route_log_file('energy_total.csv')
    #
    self.write_routing_decision('routing_decision_trace.csv')
    #
    self.write_last_sucessful_packet('last_packet.csv')
    #
    self.write_route_trace_mg('route_trace_mg.csv')
    #
    self.write_route_pheromone_log_file('route_pheromone_log.csv')
