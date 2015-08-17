#!/bin/bash

# create the log files
./compute_mean_jumps.sh
# collect the log files per experiment
cat `find . -name 'mean_jumps.csv'` >> final_mean_jumps.csv
# create the figure
R --no-save < draw_mean_jumps.R
