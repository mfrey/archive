#!/usr/bin/env python

class Settings:
  def __init__(self):
    self.xii = 1.0
    self.phi = 1.0
    self.send = 0.02
    self.recv = 0.01
    self.delta_phi = 0.1
    self.q = 0.2
    self.alpha = 1.0
    self.beta = 1.0
    self.dot_file = ""
    # plateau parameter for cubic evaporation function
    self.plateau = 3
    # slow down paramter for cubic evaporation function (slow \in (0,1))
    self.slow = 0.4
    # self reduction parameter for cubic evaporation function (reduction \in (0,1))
    self.reduction = 0.5
    # evaporation mode
    self.evaporation_mode = 0

