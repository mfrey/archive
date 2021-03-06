#!/bin/bash

# poor man's debugging (switch)
TEST=''
# the variable specifies the correspondend experiment series
EXPERIMENTS="0"
# the alpha paramter which is of interest for analysis
ALPHA_LIST="1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0"
# the beta paramter which is of interest for analysis
BETA_LIST="1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0"
REPETITIONS="1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30"

# the number of packets which were tried to submit
PACKETS=1000

for EXPERIMENT in $EXPERIMENTS; do
  for ALPHA in $ALPHA_LIST; do
	for BETA in $BETA_LIST; do
      # build up the name of the directory 
      EXPERIMENT_DIR='experiment-'$EXPERIMENT'-'$PACKETS'-'$ALPHA'-'$BETA
	  cp count_jumps.py $EXPERIMENT_DIR'/.'
	  echo cp count_jumps.py $EXPERIMENT_DIR'/.'
	  cd $EXPERIMENT_DIR
#	  rm *.csv *.py
	  python2.7 count_jumps.py
	  awk -F "," '{print '$EXPERIMENT'","$1","'$ALPHA'","'$BETA'","$3}' route_jumps.csv >> route_jumps_final.csv 
	  cd ..
#	  for REPETITION in $REPETITIONS; do
#        cp analysis.R $EXPERIMENT_DIR'/'$REPETITION'/.'
#        cd $EXPERIMENT_DIR'/'$REPETITION
#		R --no-save < analysis.R
#		rm analysis.R
#		cd ..
#	  done;
    done;
  done;
  cd ..
done;
