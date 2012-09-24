#!/usr/bin/env python

#
# Comments from MG
# The following packet structure could simplify the implementation a lot
# packet = [header|info] 
#    header = [src, dst, length]
#    info   = [visited_nodes, phi_list, xii_list, etc.]
#
# Resulting in:
# packet = [ src, dst, length | visited_nodes, phi_list, xii_list, ... ]
#
class Packet:
  def __init__(self):
    # source of the packet
    self.src = -1
    # destination of the packet
    self.dst = -1

    # packet id of the node
    self.packet_id = 0
    # list of visited nodes
    self.visited_nodes = []
    # packet length
    self.packet_length = 1.0
    # number of hops taken
    self.hop_count = -1
    # initial overall 'network' energy before transmission
    self.network_initial_xii = 0.0
    # overall 'network' energy after transmission
    self.network_final_xii = 0.0
    # transmission costs
    self.overall_transmission_costs = 0.0
    # this variable holds the energy value of all nodes (in an iteration)
    self.xii_dict = {}
    self.xii_list = []
    # this variable holds the phi value of the chosen pheromone 
    self.phi_list = []
    self.phi_dict = {}
    
  def route_to_string(self):
    return str(self.packet_id) + ',' + ','.join(map(str, self.visited_nodes))

  def set_transmission_costs(self):
    self.overall_tranmission_costs = self.network_initial_xii - self.network_final_xii

  def route_taken_mg(self):
    return (''.join(map(str,self.visited_nodes)) + "," + str(self.packet_id) + "\n")
