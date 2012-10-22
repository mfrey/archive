#!/usr/bin/env python

import random
import logging
import networkx as nx
import matplotlib.pyplot as plt

from general import settings as cfg
from network import ant as agent
from network import routingtable as rt
from network import routingtableentry as rte

module_logger = logging.getLogger(__name__)

class WirelessNetwork:
  def __init__(self):
    self.network = nx.Graph()
    self.settings = cfg.Settings()
    self.energy_consumption = []
    self.position = nx.spring_layout(self.network)
    self.routes = {}
    self.logger = logging.getLogger(__name__)
    # the handler should be overwritten by another lcass
    self.file_handler = logging.FileHandler('log_file_does_not_exist.log')
    self.logger.setLevel(logging.DEBUG)
    self.paths = {}
    self.routes = {}
    self.random_destinations = []

  def find_pair(self, depth):
    # the first node
    start = self.network.nodes()[0]
    # the last node
    end = len(self.network.nodes())

    source = random.randint(start, end)
    print "picked source node " + str(source)
    print self._find_pair(source, [], 3)
    print self.random_destinations


  def _find_pair(self, source, blacklist, depth):
    depth = depth - 1
    blacklist.append(source)

    if depth > 0:
      for node in nx.neighbors(self.network, source):
        if node not in blacklist:
          self._find_pair(node, blacklist, depth)
    else:
      self.random_destinations.append(source)


  def setup(self):
    for n in self.network.nodes():
      # set up routing table
      self.network.node[n]['routing table'] = rt.RoutingTable()
      self.network.node[n]['routing table'].logger.addHandler(self.file_handler)
      # set up energy level
      self.network.node[n]['energy'] = self.settings.xii
      # log it
      self.log_energy_consumption(0, n, self.network.node[n]['energy'])
      # set node to active
      self.network.node[n]['active'] = True
      # initialize the last packets array
      self.network.node[n]['last packets'] = []

  def is_active(self, node):
    return self.network.node[node]['active']

  def get_neighbors(self,node):
    return self.network.neighbors(node)

  def initialize_route_discovery(self, source, destination):
    paths = nx.all_simple_paths(self.network, source, destination, 7)
    for path in paths:
      if not self.routes.has_key((source,destination)):
        self.routes[(source,destination)] = [path]
      else:
        self.routes[(source,destination)].append(path)

      for index, node in enumerate(path):
        if index != len(path)-1:
          # forward ant entry
          entry  = self.createRoutingTableEntry(path[index+1], node, source, self.settings.phi, self.settings.xii)
          self.updateRoutingTable(0, entry)
          # backward ant entry
          entry  = self.createRoutingTableEntry(node, path[index+1], destination, self.settings.phi, self.settings.xii)
          self.updateRoutingTable(0, entry)

  def set_initial_phi_value(self, src, dst):
    # get all shortest paths between a source and a destination
    paths = nx.all_shortest_paths(self.network, source=src, target=dst)
    # iterate over the paths
    for path in paths:
      for index, node in enumerate(path):
        # check if it is the last element in the list
        if index != (len(path)-1):
          node_i = node
          node_j = path[index+1]
          self.logger.debug(' update phi for (' + str(node_i) + ', ' + str(node_j) + ', ' + str(dst) + ')')

          phi = 2 * self.network.node[node]['routing table'].get_phi(0, node_i, node_j, dst)
          self.network.node[node]['routing table'].set_phi(0, node_i, node_j, dst, phi)

  def updateRoutingTable(self, packet, entry):
    self.network.node[entry.node_i]['routing table'].add(packet, entry)

  def get_routingtable_trace(self):
    result = []
    for n in self.network.nodes():
      # iterate over the packet entries in the routing table
      for packet, entries in sorted(self.network.node[n]['routing table']._table.items()):
        # iterate over the routing table entries of a packet
        for entry in entries:
          routing_table_entry = self.network.node[n]['routing table']._table[packet][entry]
          result.append([packet, routing_table_entry.node_i, routing_table_entry.node_j, routing_table_entry.destination, routing_table_entry.phi, routing_table_entry.xii])
    return result

  def dumpRoutingTable(self):
    for n in self.network.nodes():
      # iterate over the packet entries in the routing table
      for packet, entries in sorted(self.network.node[n]['routing table']._table.items()):
        # iterate over the routing table entries of a packet
        for entry in entries:
          routing_table_entry = self.network.node[n]['routing table']._table[packet][entry]
          print str(packet) + ', ' + str(routing_table_entry.node_i) + ', ' + str(routing_table_entry.node_j) + ', ' + str(routing_table_entry.destination) + ', ' + str(routing_table_entry.phi) + ', ' + str(routing_table_entry.xii)

  def createRoutingTableEntry(self, node_i, node_j, destination, phi, xii):
    self.logger.debug('create routing table entry for node ' + str(node_i) + ' to node ' + str(destination) + ' via ' + str(node_j) + ' with phi ' + str(phi) + ' and energy value ' + str(xii))
    entry = rte.RoutingTableEntry()
    entry.node_i      = node_i
    entry.node_j      = node_j
    entry.destination = destination
    entry.phi         = phi 
    entry.xii         = xii
    return entry

  def getNeighborsTowardsDestination(self, packet, source, destination):
    result = []
    for entry in self.network.node[source]['routing table'].table[packet]:
      if entry.destination == destination:
        result.append(entry.node_j)
    return result

  def get_energy_status(self, node):
	return self.network.node[node]['active']

  def set_energy_status(self, node, status):
	self.network.node[node]['active'] = status;

  def get_energy_level(self, node):
	return self.network.node[node]['energy']

  def set_energy_level(self, packet, node, energy):
    # iterate over the neighbors (predecessors and successors)
    for n in self.network.neighbors(node):
      self.logger.debug('set energy level of node ' + str(node) + " at node " + str(n) + " and packet " + str(packet))
      self.network.node[n]['routing table'].set_energy_level(packet, node, energy)
    self.network.node[node]['energy'] = energy;

  def get_phi(self, packet, sender, node, destination):
	return self.network.node[sender]['routing table'].get_phi(packet, sender, node, destination)

  def set_phi(self, packet, sender, node, destination, phi):
    self.logger.debug('set phi of edge(' + str(sender) + ", " + str(node) + ") to " + str(phi) + ' and destination is ' + str(destination))
    # TODO create a settings constant for threshold
    if phi < 0.09:
      inactive_flag = True
      # we have to check if phi(x,y) is also below the given threshold and not onlye phi(y,x) 
      for entry in self.network.node[node]['routing table']._table[packet].keys():
        if entry[0] == node:
          # if there is an entry with an higher pheromone value 
          if self.network.node[node]['routing table'].get_phi(packet, entry[0], entry[1], entry[2]) > 0.09:
            self.logger.debug('found entry(' + str(entry[0]) + "," + str(entry[1]) + "," + str(entry[2]) + ") with a pheromone value above threshold")
            inactive_flag = False
            break

      if inactive_flag:
        self.logger.debug('set node ' + str(node) + " to inactive")
        # set the node to inactive
        self.network.node[node]['active'] = False
      # invalidate the link
      phi = 0.0 
    self.network.node[sender]['routing table'].set_phi(packet, sender, node, destination, phi)

  def reset_phi(self, packet, sender):
    self.network.node[sender]['routing table'].reset_phi(packet)

  def has_path(self, node, destination):
	return nx.has_path(self.network, node, destination)

  def log_energy_consumption(self, packet, node, energy):
    self.energy_consumption.append([packet, node, energy])

  def get_network_energy(self):
    result = 0.0
    for n in self.network.nodes():
      result += self.network.node[n]['energy']
    return result

  def get_network_energy_as_dict(self):
    result = {}
    for n in self.network.nodes():
      result[n] = self.network.node[n]['energy']
    return result

  def draw(self):
    nodecolor = "#99CC00"
    nx.draw_spectral(self.network, node_color=nodecolor)
    plt.savefig("path.png")

  # returns the node with the highest phi value towards the destination
  def get_highest_phi(self, packet, source, nodes, destination):
    phi = 0
    result = -1
    for node in nodes:
	  if self.network.node[node]['active']: 
		new_phi = self.network.node[source]['routing table'].get_phi(packet, source, node, destination)
		if phi <= new_phi:
		  phi = new_phi
		  result = node
    return result
              
  def find_all_paths(self, source, destination, cutoff):
    """ The function finds all simple paths between two nodes.

    Parameters
    ----------
    source : The source node.

    destination : The destination node.

    cutoff : The maximum path length to search.

    Notes
    -----
    The function basically wraps the nx.all_simple_paths() method
    of the networkx package and adds support for reading/writing
    already found paths between a source/destination of previous
    runs of this script. If at all, this only makes sense if the 
    graphs are below a certain size/order. As stated in the 
    documentation of networkx, all single paths can be found
    at best in O(V+E) and at worst in O(n!) (where n denotes |V|).
    """
    key = (source, destination)

    # check if all paths have been previously found
    if key not in self.paths:
      # iterate over the results
      for path in nx.all_simple_paths(self.network, source, destination, cutoff):
        self.paths[key].append(path)

def main():
    network = nx.Graph()
    network.add_nodes_from([1,8]);
    network.add_edges_from([(1,2),(1,3),(1,4),(2,5),(5,7),(7,8),(3,6),(6,8),(4,8)])
    
    w = WirelessNetwork()
    w.network = network
    w.draw()

if __name__ == "__main__":
    main()
