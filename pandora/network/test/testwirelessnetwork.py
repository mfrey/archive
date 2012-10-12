#!/usr/bin/env python

import unittest
import networkx as nx

from network.wirelessnetwork import WirelessNetwork

class TestWirelessNetworkFunctions(unittest.TestCase):
  def setUp(self):
    self.network = WirelessNetwork()
    # set up a simple network
    network = nx.Graph()
    # add nodes
    network.add_nodes_from([1,4])
    # add edges
    network.add_edges_from([(1,2),(1,3),(2,4),(3,4)])
    # set the network
    self.network.network = network

  
  def test_initialize_route_discovery(self):
    # source node
    source = 1
    # destination node 
    destination = 4
    #
    self.network.setInitialRoutingTable()
    #
    self.network.setInitialEnergyLevel(10.0)
    # initialize route discovery (test the method)
    self.network.initialize_route_discovery(source, destination)


#  def test_add(self):


if __name__ == '__main__':
  unittest.main()
