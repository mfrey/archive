#!/usr/bin/env python

import os

class CountJumps:
  def __init__(self):
	self.routes = 2

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
          result = repetition + "," + str(self.routes) + "," + str(switch_counter)
          output.write("%s\n" % result)
    output.close()

generator = CountJumps()
generator.read(".")

