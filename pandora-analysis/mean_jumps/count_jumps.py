#!/usr/bin/env python

import os
import argparse

class CountJumps:
  def __init__(self):
    self.routes = 2
    self.alpha = 1.0
    self.beta = 1.0
    self.experiment = 0

  def read(self, directory):
    output = open("route_jumps.csv", "a")
    result = ""
    for root, sub_folders, files in os.walk(directory):
      for file in files:
        if file == "routing_decision_trace.csv":
          csv = open(str(root) + "/" + str(file),"r")
          previous_route = 0
          switch_counter = 0
          for line in csv:
            # remove the new line
            result = line.rstrip('\n')
            # split the string
            result = result.split(",")
            if previous_route != 0:
              if previous_route != result[1]:
                switch_counter += 1
                previous_route = result[1]
            else:
              previous_route = result[1]
          csv.close()
          repetition = str(root).replace("./","")
          result = str(self.experiment) + ',' + str(self.alpha) + ',' + str(self.beta) + ',' + repetition + "," + str(self.routes) + "," + str(switch_counter)
          output.write("%s\n" % result)
    output.close()

def main():
  parser = argparse.ArgumentParser(description='script for generating route jump statistics')
  parser.add_argument('-r', dest='routes', type=int, default=10, action='store', help='number of routes')
  parser.add_argument('-a', dest='alpha', type=float, default=1.0, action='store', help='alpha')
  parser.add_argument('-e', dest='experiment', type=int, default=0, action='store', help='experiment id')
  parser.add_argument('-b', dest='beta', type=float, default=1.0, action='store', help='beta')

  arguments = parser.parse_args()

  generator = CountJumps()
  generator.routes = arguments.routes
  generator.alpha = arguments.alpha
  generator.beta = arguments.beta
  generator.experiment = arguments.experiment
  generator.read(".")

if __name__ == "__main__":
  main()
