#!/usr/bin/env python

import copy
import logging

from network import routingtableentry as rte

module_logger = logging.getLogger(__name__)

class RoutingTable:
  def __init__(self):
    self._table = {}
    self.logger = logging.getLogger(__name__)
    self.logger.setLevel(self.logger.DEBUG)

  def add(self, packet, entry):
    key= (entry.node_i, entry.node_j, entry.destination)
    self._table.setdefault(packet, {})[key] = entry

  def create_key(self, entry):
    return ((entry.node_i, entry.node_j, entry.destination))

  @property
  def table(self):
	return {p:d.values() for (p,d) in self._table.items()}

  def get(self, packet):
    return self._table[packet]

  def __str__(self):
    result = str()
    for key in sorted(self._table.keys()):
      print key
      for entry in self._table[key]:
        result = result + str(key) + "," + str(entry) + "\n"
    return result

  def get_phi(self, packet, node_i, node_j, destination):
    try:
	  key = (node_i, node_j, destination)
	  return self._table[packet][key].phi
    except KeyError:
	  self.logger.error(self)

  def set_phi(self, packet, node_i, node_j, destination, phi):
    try:
	  key = (node_i, node_j, destination)
	  self._table[packet][key].phi = phi
    except KeyError:
	  self.logger.error("key error @ RoutingTable:set_phi")
	  self.logger.error(self)

  def reset_phi(self, packet):
    try:
      if len(self._table.keys()) > 0: 
        for key in self._table[packet].keys():
          self._table[packet][key].phi = 0.0
    except KeyError:
	  self.logger.error("key error @ RoutingTable:set_phi")
	  self.logger.error(self)

  def set_energy_level(self, packet, node, energy):
    self.logger.debug('set energy level of node ' + str(node) + " and packet " + str(packet) + " to a energy level of "  + str(energy))
    try: 
      if len(self._table.keys()) > 0: 
#        self.logger.debug('keys are ' + str(self._table[packet].keys()))
        for key in self._table[packet].keys():
#          self.logger.debug('key is ' + str(key))
#          self.logger.debug('entries are ' + str(self._table[packet][key]))
          # the second entry is the node_j entry in the key
          if key[1] == node: 
             if packet > 1:
           #    self.logger.debug('energy level of ' + str(self._table[packet-1][key].xii) + " and " + str(self._table[packet][key].xii))
           #    self.logger.debug('set energy level to ' + str(energy))
               self._table[packet][key].xii = energy
           #    self.logger.debug('energy level of ' + str(self._table[packet-1][key].xii) + " and " + str(self._table[packet][key].xii))
    except KeyError:
	  self.logger.error("key error")
	  traceback.print_exc(file=sys.stdout)

  def duplicate_entries(self, packet):
    try:
      self.logger.debug('duplicate packet ' + str(packet) + ' of packet ' + str(packet -1))
      if len(self._table.keys()) > 0: 
        for key in self._table[(packet-1)].keys(): 
          self.logger.debug('duplicate packet ' + str(packet) + ' with key ' + str(key))
		  # get the current entry
          entry = copy.deepcopy(self._table[(packet-1)][key])
          self.add(packet, entry)
    except KeyError:
      self.logger.error("key error")
      traceback.print_exc(file=sys.stdout)
