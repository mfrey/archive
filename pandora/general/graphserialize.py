#!/usr/bin/env python

import logging
import networkx as nx
import pygraphviz as pgv

from networkx.readwrite import json_graph

module_logger = logging.getLogger(__name__)

class GraphSerialize:
  def __init__(self):
    self.logger = logging.getLogger(__name__)
    self.logger.setLevel(logging.FATAL)

  def read_dot_file(self, filename):
    graph = pgv.AGraph(filename)
    network = nx.Graph(nx.from_agraph(graph))
    mapping = dict([(n,int(n)) for n in network.nodes()])
    result = nx.relabel_nodes(network, mapping)
    return result


  def write_json_graph(self, file_name, network):
    """ Write a JSON representation of a graph to a file. 

    Parameters
    ----------
    file_name : The name of the file to which the JSON representation will be written

    network: The graph which will be written

    """
    try:
      export_file = open(file_name, "w")
      export_file.write(json_graph.dumps(network))
      export_file.close()
    except IOError:
      self.logger.fatal('an IO error exception occurred while writing a JSON representation of a graph')

  def read_json_graph(self, file_name):
    """ Read a JSON representation of a graph from a file. 

    Parameters
    ----------
    file_name : The name of the file of which the JSON representation will be read

    """
    try:
      import_file = open(file_name, "r")
      content = import_file.read()
      network = json_graph.loads(content)
      import_file.close()
      return network
    except IOError:
      self.logger.fatal('an IO error exception occurred while reading a JSON representation of a graph')
