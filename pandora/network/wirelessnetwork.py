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

  def find_pair(self):
    # the first node
    start = self.network.nodes()[0]
    # the last node
    end = len(self.network.nodes())

    source = random.randint(start, end)

    while True:
      destination = random.randint(start, end)

      if destination != source:
        # check if the destination is an immediate neighbor of the source node
        if destination not in self.network.neighbors(source):
          # check if there is an path between source and destination
          if nx.has_path(self.network, source, destination):
            return (source, destination)

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

  def rd(self, source, destination):
    # build up the initial list of neighbors
    neighbors = [nx.neighbors(self.network, source)]
    # set the previous node initially to the source node
    previous = [source]
    # set the index 
    index = 0

    while neighbors:
      node_j = previos.pop()
      neighbor = neighbors.pop()

      for index, node_i in enumerate(neighbor):
        entry = (node_i, node_j, source)
        print entry

        if node_i != destination:
          if index == len(neighbor)-1:
            next_neighbors = []
            for i,j in enumerate(neighbors):
              for k,l in enumerate(j):
                temp_neighbors = nx.neighbors(self.network, l)
                temp_neighbors = filter(lambda a: a != previous[i], temp_neighbors)
                print temp_neighbors
                next_neighbors.append(temp_neighbors)
            previous = neighbors
            neighbors.append(next_neighbors)

      print "bar "  + str(neighbors)
          
 

  def is_active(self, node):
    return self.network.node[node]['active']

  def get_neighbors(self,node):
    return self.network.neighbors(node)

  def initialize_route_discovery(self, source, destination):
    # create an ant agent
    pkt = agent.Ant()
    pkt.sequence_number = 0
    pkt.source = source
    pkt.destination = destination
    pkt.xii = self.settings.xii
    pkt.fant = True
    # add the initial packet to the source node
    self.network.node[source]['last packets'].append(0)
    # send forward ant agents
    for n in self.network.neighbors_iter(source):
      self.logger.debug('send forward agent to node ' + str(n))
      self.find_route(source, pkt, n) 
      pkt.sequence_number = pkt.sequence_number + 1

  def find_route(self, previous_hop, packet, node):
    # we have finally received the destination node
    if node == packet.destination:
       self.logger.debug('finally at destination node ' + str(node))
       if packet.fant == True:
         self.logger.debug('packet ' + str(packet.sequence_number) + ' at destination node ' + str(node))
         new_pkt = agent.Ant()
         # switch source and destination in the agent
         new_pkt.destination = packet.source
         new_pkt.source = node 
         # change the type of the packet
         new_pkt.fant = False
         new_pkt.xii  = self.network.node[node]['energy']
         # set the packet id
         new_pkt.sequence_number = packet.sequence_number + 1
         self.logger.debug('create new ant agent ' + str(new_pkt))
         #
         entry  = self.createRoutingTableEntry(node, previous_hop, new_pkt.destination, self.settings.phi, new_pkt.xii)
	     # add the entry to the routing table
         self.updateRoutingTable(0, entry)
         # append the packet id to the 'last packets' vector at the destination
         self.network.node[node]['last packets'].append(new_pkt.sequence_number)
         # send the packet back
         for n in self.network.neighbors(node): 
           self.find_route(node, new_pkt, n)
         # todo: update costs
       else:
         self.logger.debug('add entry (node_i, node_j, dst) ' + str(node) + ', ' + str(previous_hop) + ', ' + str(packet.source))
         xii  = self.network.node[node]['energy']
         entry  = self.createRoutingTableEntry(node, previous_hop, packet.source, self.settings.phi, xii)
	     # add the entry to the routing table
         self.updateRoutingTable(0, entry)

    else:
      # check if we have seen the packet already
      if packet.sequence_number not in self.network.node[node]['last packets']:
        self.logger.debug('node ' + str(node) + " received packet " + str(packet.sequence_number))
        entry  = self.createRoutingTableEntry(node, previous_hop, packet.source, self.settings.phi, packet.xii)
        self.logger.debug('add entry (node_i, node_j, dst) ' + str(node) + ", " + str(previous_hop) + ", " + str(packet.source))
	    # add the entry to the routing table
        self.updateRoutingTable(0, entry)
        # append the packet id to the 'last packets' vector at the destination
        self.network.node[node]['last packets'].append(packet.sequence_number)
        # update the energy costs in the packet
        packet.xii = self.network.node[node]['energy'] 
        # send the packet to the neighbors of the node
        for n in self.network.neighbors(node): 
          # don't forward the packet to the previous hop
          if n != previous_hop:
            self.find_route(node, packet, n)
        # todo: update costs

      # log the duplicate 
      else:
        self.logger.debug('received duplicate packet ' + str(packet.sequence_number) + " at node " + str(node))
        self.logger.debug('received duplicate packets are ' + str(self.network.node[node]['last packets']))

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


  def dummy_func(self, source, destination, key):
    for entry in self.network.node[source]['routing table'].get(0):
      # check if the 'destination' in the entry matches the desired destination
      if entry[2] == destination:
		if source != key[0]:
		  routes_to_append = []
		  # append the node to previous entries (if necessary)
		  for index, route in enumerate(self.routes[key]):
			# check if the current node is in the current route 
			if entry[0] in route:
			  # determine the position of the last entry
			  last_entry = len(route) - 1

			  # check if the current node is the last entry in the current route 
			  if entry[0] == route[last_entry]: 
				# add entry 
				self.routes[key][index].append(entry[1])
			  # TODO: CHECK!
			  else:
				# get the index of the last occurence of the node 
				last_entry = [i for i, v in enumerate(route) if v == entry[0]]
				# build up a new path (we certainly 'hope' that there is only one occurence of the node in the list)
				new_route = [v for i, v in enumerate(route) if i <= ((last_entry[0]))]
				# append the node
				new_route.append(entry[1])
				# append it 
				routes_to_append.append(new_route)

#		  for route in routes_to_append: 
#			self.routes[key].append(route)
        # the else statement will only executed once per source/destination pair            
		else:
		  if not self.routes.has_key(key):
			self.routes[key] = [[entry[0], entry[1]]]
		  else:
			self.routes[key].append([entry[0], entry[1]])
        # recursive call
		self.dummy_func(entry[1], destination, key)
              
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
