#!/usr/bin/env python

import copy
import logging

from network import routingtableentry as rte

logging.basicConfig(filename='example.log',level=logging.DEBUG)

class RoutingTable:
  def __init__(self):
    self._table = {}

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
	  logging.error(self)

  def set_phi(self, packet, node_i, node_j, destination, phi):
    try:
	  key = (node_i, node_j, destination)
	  self._table[packet][key].phi = phi
    except KeyError:
	  logging.error("key error @ RoutingTable:set_phi")
	  logging.error(self)

  def reset_phi(self, packet):
    try:
      if len(self._table.keys()) > 0: 
        for key in self._table[packet].keys():
          self._table[packet][key].phi = 0.0
    except KeyError:
	  logging.error("key error @ RoutingTable:set_phi")
	  logging.error(self)

  def set_energy_level(self, packet, node, energy):
    logging.debug('set energy level of node ' + str(node) + " and packet " + str(packet) + " to a energy level of "  + str(energy))
    try: 
      if len(self._table.keys()) > 0: 
#        logging.debug('keys are ' + str(self._table[packet].keys()))
        for key in self._table[packet].keys():
#          logging.debug('key is ' + str(key))
#          logging.debug('entries are ' + str(self._table[packet][key]))
          # the second entry is the node_j entry in the key
          if key[1] == node: 
             if packet > 1:
           #    logging.debug('energy level of ' + str(self._table[packet-1][key].xii) + " and " + str(self._table[packet][key].xii))
           #    logging.debug('set energy level to ' + str(energy))
               self._table[packet][key].xii = energy
           #    logging.debug('energy level of ' + str(self._table[packet-1][key].xii) + " and " + str(self._table[packet][key].xii))
    except KeyError:
	  logging.error("key error")
	  traceback.print_exc(file=sys.stdout)

  def duplicate_entries(self, packet):
    try:
      logging.debug('duplicate packet ' + str(packet) + ' of packet ' + str(packet -1))
      if len(self._table.keys()) > 0: 
        for key in self._table[(packet-1)].keys(): 
          logging.debug('duplicate packet ' + str(packet) + ' with key ' + str(key))
		  # get the current entry
          entry = copy.deepcopy(self._table[(packet-1)][key])
          self.add(packet, entry)
    except KeyError:
      logging.error("key error")
      traceback.print_exc(file=sys.stdout)
