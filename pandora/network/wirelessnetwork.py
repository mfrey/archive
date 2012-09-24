#!/usr/bin/env python

import logging
import networkx as nx
import matplotlib.pyplot as plt

logging.basicConfig(filename='example.log',level=logging.DEBUG)

from general import settings as cfg
from network import ant as agent
from network import routingtable as rt
from network import routingtableentry as rte

class WirelessNetwork:
  def __init__(self):
    self.network = nx.Graph()
    #self.network = nx.DiGraph()
    self.settings = cfg.Settings()
    self.energy_consumption = []
    self.position = nx.spring_layout(self.network)
    self.routes = {}

  def setInitialEnergyLevel(self,xii):
    for n in self.network.nodes():
      self.network.node[n]['energy'] = xii
      self.log_energy_consumption(0, n, xii)
      self.network.node[n]['active'] = True
      # todo: move somewhere else
      self.network.node[n]['last packets'] = []

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
      logging.debug('send forward agent to node ' + str(n))
      self.find_route(source, pkt, n) 

  def find_route(self, previous_hop, packet, node):
    # we have finally received the destination node
    if node == packet.destination:
       logging.debug('finally at the destination node ' + str(node) + ' at destination node ' + str(node))
       if packet.fant == True:
         logging.debug('packet ' + str(packet.sequence_number) + ' at destination node ' + str(node))
         new_pkt = agent.Ant()
         # switch source and destination in the agent
         new_pkt.destination = packet.source
         new_pkt.source = node 
         # change the type of the packet
         new_pkt.fant = False
         new_pkt.xii  = self.network.node[node]['energy']
         #
         entry  = self.createRoutingTableEntry(node, previous_hop, new_pkt.destination, self.settings.phi, new_pkt.xii)
	     # add the entry to the routing table
         self.updateRoutingTable(0, entry)
         # set the packet id, TODO check (bug)
         new_pkt.sequence_number = packet.sequence_number + 1
         # append the packet id to the 'last packets' vector at the destination
         self.network.node[node]['last packets'].append(new_pkt.sequence_number)
         # send the packet back
         for n in self.network.neighbors(node): 
           self.find_route(node, new_pkt, n)
         # todo: update costs
       else:
         logging.debug('add entry (node_i, node_j, dst) ' + str(node) + ', ' + str(previous_hop) + ', ' + str(packet.source))
         xii  = self.network.node[node]['energy']
         entry  = self.createRoutingTableEntry(node, previous_hop, packet.source, self.settings.phi, xii)
	     # add the entry to the routing table
         self.updateRoutingTable(0, entry)

    else:
      # check if we have seen the packet already
      if packet.sequence_number not in self.network.node[node]['last packets']:
        logging.debug('node ' + str(node) + " received packet " + str(packet.sequence_number))
        entry  = self.createRoutingTableEntry(node, previous_hop, packet.source, self.settings.phi, packet.xii)
        logging.debug('add entry (node_i, node_j, dst) ' + str(node) + ", " + str(previous_hop) + ", " + str(packet.source))
	    # add the entry to the routing table
        self.updateRoutingTable(0, entry)
        # append the packet id to the 'last packets' vector at the destination
        self.network.node[node]['last packets'].append(packet.sequence_number)
        # update the energy costs in the packet
        packet.xii = self.network.node[node]['energy'] 
        # send the packet to the neighbors of the node
        for n in self.network.neighbors(node): 
          self.find_route(node, packet, n)
        # todo: update costs

      # log the duplicate 
      else:
        logging.debug('received duplicate packet ' + str(packet.sequence_number) + " at node " + str(node))
        logging.debug('received duplicate packets are ' + str(self.network.node[node]['last packets']))

  def setInitialRoutingTable(self):
    for n in self.network.nodes():
      self.network.node[n]['routing table'] = rt.RoutingTable()

  def updateRoutingTable(self, packet, entry):
    self.network.node[entry.node_i]['routing table'].add(packet, entry)

  def compute_path_phi(self, packet, nodes, destination):
    #print nodes
    phi = 0.0
# TODO: this needs to be fixed because of our strange setup (undirected graphs)
#    for index, value in enumerate(nodes):
#      if (index + 1) != len(nodes):
#        temp = self.network.node[nodes[index]]['routing table']._table[packet][(nodes[index], nodes[index+1], destination)].phi
#        phi += float(temp)
    return phi

  def getRoutingTableTrace(self):
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
    logging.debug('create routing table entry for node ' + str(node_i) + ' to node ' + str(destination) + ' via ' + str(node_j) + ' with phi ' + str(phi) + ' and energy value ' + str(xii))
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
      logging.debug('set energy level of node ' + str(node) + " at node " + str(n) + " and packet " + str(packet))
      self.network.node[n]['routing table'].set_energy_level(packet, node, energy)
    self.network.node[node]['energy'] = energy;

  def get_phi(self, packet, sender, node, destination):
	return self.network.node[sender]['routing table'].get_phi(packet, sender, node, destination)

  def set_phi(self, packet, sender, node, destination, phi):
    logging.debug('set phi of edge(' + str(sender) + ", " + str(node) + ") to " + str(phi))
    # TODO create a settings constant for threshold
    if phi < 0.09:
    	self.network.node[node]['active'] = False
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

def main():
  network = nx.Graph()
  network.add_nodes_from([1,8]);
  network.add_edges_from([(1,2),(1,3),(1,4),(2,5),(5,7),(7,8),(3,6),(6,8),(4,8)])

  w = wifi.WirelessNetwork()
  w.network = network
  w.draw()

if __name__ == "__main__":
  main()
