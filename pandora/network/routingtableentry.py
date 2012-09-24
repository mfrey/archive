#!/usr/bin/env python

class RoutingTableEntry:
  def __init__(self):
    # current node
    self.node_i = 1
    # next hop
    self.node_j = 1
    self.destination = 1
    self.phi = 1.0
    self.xii = 1.0
    # shortest hop distance (todo)
    self.hop_count = 1.0

  def __str__(self):
    return str(self.node_i) + "," + str(self.node_j) + "," + str(self.destination) + "," + str(self.phi) + "," + str(self.xii)

  def __cmp__(self,compare):
    # we do not compare phi and xii 
    return (self.node_i == compare.node_i and self.node_j == compare.node_j and self.destination == compare.destination)
