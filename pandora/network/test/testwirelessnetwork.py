#!/usr/bin/env python

import unittest

from general.settings import Settings
from general.graphserialize import GraphSerialize
from network.wirelessnetwork import WirelessNetwork
from network.routingtable import RoutingTable

class TestWirelessNetworkFunctions(unittest.TestCase):
  def setUp(self):
    self.network = WirelessNetwork()
    # create a settings object
    self.settings = Settings()
    # set the initial energy level of a node
    self.settings.xii = 10.0
    # store the settings
    self.network.settings = self.settings
    serialize = GraphSerialize()
    # set up a simple network
    self.network.network =  serialize.read_dot_file('grid.dot')

  def test_setup(self):
    # test the setup method
    self.network.setup()
    # every node should have a routing table
    for node in self.network.network.nodes():
      # check if a routing table instance was created
      self.assertIsInstance(self.network.network.node[node]['routing table'], RoutingTable)
      # the energy level should match the energy level defined in the settings
      self.assertIs(self.network.network.node[node]['energy'], self.settings.xii)
      # check if the active flag was set correctly
      self.assertTrue(self.network.network.node[node]['active'])
      # check if the array was initialized
      self.assertIsNotNone(self.network.network.node[node]['last packets'])
  
  def test_initialize_route_discovery(self):
    # source node
    source = 1
    # destination node 
    destination = 4
    # set up the network
    self.network.setup()
    # initialize route discovery (test the method)
    self.network.initialize_route_discovery(source, destination)
  #  self.network.dumpRoutingTable()


  def test_fck(self):
    src, dst = 4, 5
    # set up the network
    self.network.setup()
    self.network.initialize_route_discovery(src, dst)
    self.network.dummy_func(src, dst, (src,dst))
    print self.network.routes
    

if __name__ == '__main__':
    unittest.main()
