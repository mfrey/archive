#!/usr/bin/env python

import networkx as nx
import pygraphviz as pgv

class DotFileReader:
  def initialize(self):
    self.graph = pgv.AGraph()

  def read_dot_file(self, filename):
    self.graph = pgv.AGraph(filename)
    network = nx.Graph(nx.from_agraph(self.graph))
    mapping = dict([(n,int(n)) for n in network.nodes()])
    result = nx.relabel_nodes(network, mapping)
    return result



