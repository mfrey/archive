#!/bin/bash

# create the log files
./compute_mean_packets.sh
# collect the log files per experiment
cat `find . -name 'mean_packets.csv'` >> final_mean_packets.csv
# create the figure
R --no-save < draw_mean_packets.R
