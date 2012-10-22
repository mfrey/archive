#!/usr/bin/env python

import logging
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
    # set file handler
    self.network.file_handler = logging.FileHandler('unittest.log')
    # enable file handler
    self.network.logger.addHandler(self.network.file_handler)

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
    src, dst = 4, 10
    # set up the network
    self.network.setup()
    # initialize route discovery (test the method)
    self.network.initialize_route_discovery(src, dst)

  def test_fck(self):
    src, dst = 4, 10
    # set up the network
    self.network.setup()
    self.network.initialize_route_discovery(src, dst)
    self.network.dummy_func(src, dst, (src,dst))
    print self.network.routes
    
  def test_rd(self):
    src, dst = 4, 10 
    self.network.setup()
    self.network.rd(4,10)

if __name__ == '__main__':
    unittest.main()
