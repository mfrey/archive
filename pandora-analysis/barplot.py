#!/usr/bin/env python

import csv
import random

class Helper:
  def cumulated_sum(self,iterable):
    iterable= iter(iterable)
    s = iterable.next()
    yield s
    for c in iterable:
      s = s + c
      yield s


class Node:
  def __init__(self):
	self.node = -1
	self.xii = 10.0
	self.phi = 1.0

  def create_row(self, iteration, probablity):
	result = [ iteration, self.node, probablity, self.phi, self.xii ]
	return result

class GenerateBar:
  def __init__(self):
	self.alpha = 1.0
	self.beta = 1.0
	self.xii = 10.0
	self.phi = 1.0
	self.delta_phi = 5.0
	self.q = 0.1
	self.helper = Helper()

  def generate(self, packets):
	result = []
	a = Node()
	a.node = 2
	b = Node()
	b.node = 3

	for packet in range(packets):
	  prob_a = (a.phi**self.alpha) * (a.xii**self.beta)
	  prob_b = (b.phi**self.alpha) * (b.xii**self.beta)
	  prob_sum = prob_a + prob_b
	  prob_a = prob_a / prob_sum
	  prob_b = prob_b / prob_sum
	  result.append(a.create_row(packet, prob_a))
	  result.append(b.create_row(packet, prob_b))
	  distribution = [prob_a, prob_b]
	  cumulatedDensity = list(self.helper.cumulated_sum(distribution))
	  q = random.uniform(0,1)
	  path = 0
	  while q > cumulatedDensity[path]:
		path += 1

	  # update the probablities
	  if path == 0:
	    a.phi = self.increasePheromoneValue(a.phi)
	    b.phi = self.decreasePheromoneValue(b.phi)
   	    # update the xii values
	    a.xii = a.xii - 0.03
	    b.xii = b.xii - 0.01
	  else:
	    b.phi = self.increasePheromoneValue(b.phi)
	    a.phi = self.decreasePheromoneValue(a.phi)
   	    # update the xii values
	    b.xii = b.xii - 0.03
	    a.xii = a.xii - 0.01

	return result

  def increasePheromoneValue(self, phi):
	return (phi + self.delta_phi)

  def decreasePheromoneValue(self, phi):
	return (phi * (1 - self.q))

def main():
  g = GenerateBar()
  result = g.generate(30)
  log_file = csv.writer(open("barplot-"+str(g.alpha)+"-"+str(g.beta)+".csv", "w"), delimiter=',')
  for entry in result:
    log_file.writerow(entry)

if __name__ == "__main__":
    main()
