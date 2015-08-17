#!/usr/bin/env python

class Ant:
  def __init__(self):
    self.sequence_number = -1
    self.source = -1
    self.destination = -1
    self.xii = -1.0
    self.fant = True

  def __str__(self):
    ant = "bant"

    if self.fant:
      ant = "fant" 
 
    return 'ant type: ' + ant + ', seq: ' + str(self.sequence_number) + ', src: ' + str(self.source) + ', dst: ' + str(self.destination) + ',  xii: ' + str(self.xii) 

