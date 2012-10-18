#!/usr/bin/env python

import random
import unittest
import networkx as nx

from general.graphserialize import GraphSerialize

class TestGraphSerialize(unittest.TestCase):
  def setUp(self):
    self.network = nx.gnm_random_graph(100, 5, 42, False)

  def test_json_graph(self):
    export = GraphSerialize()
    export.write_json_graph("unittest.json", self.network)
    read_network = export.read_json_graph("unittest.json")
    # check if the written file has the number of nodes (and identifier)
    self.assertEqual(self.network.nodes(), read_network.nodes())
    # check if the first node has the same neighbors
    self.assertEqual(self.network.neighbors(1), read_network.neighbors(1))
    # pick a random node
    random_node = int(random.uniform(2,100))
    # check if a randomly picked node has the same neighbors 
    self.assertEqual(self.network.nodes(random_node), read_network.nodes(random_node))

if __name__ == '__main__':
  unittest.main()
