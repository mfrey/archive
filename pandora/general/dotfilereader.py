#!/usr/bin/env python

import networkx as nx
import pygraphviz as pgv

class DotFileReader:
  def initialize(self):
    self.graph = pgv.Agraph()

  def read_dot_file(self, filename):
    self.graph = pgv.AGraph(filename)
    return nx.Graph(nx.from_pydot(self.graph))



