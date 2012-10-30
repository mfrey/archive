#!/bin/bash

# The script generates a file which holds the mean last packet
# of the current experiment. The file is named 'mean_packet.log'. 

# the variable specifies the correspondend experiment series
EXPERIMENTS="1"
# the alpha paramter which is of interest for analysis
#ALPHA_LIST="1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0"
# the beta paramter which is of interest for analysis
#BETA_LIST="1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0"
ALPHA_LIST="1.0 1.25 1.5 1.75 2.0 2.25"
BETA_LIST="1.0 1.25 1.5 1.75 2.0 2.25 2.5"
REPETITIONS="1  2  3  4  5  6  7  8  9 10"
# the number of packets which were tried to submit
PACKETS=1000

for EXPERIMENT in $EXPERIMENTS; do
  for ALPHA in $ALPHA_LIST; do
    for BETA in $BETA_LIST; do
      # build up the name of the directory 
      EXPERIMENT_DIR='experiment-'$EXPERIMENT'-'$PACKETS'-'$ALPHA'-'$BETA
      for REPETITION in $REPETITIONS; do
        cp count_jumps.py $EXPERIMENT_DIR'/'$REPETITION
        cd $EXPERIMENT_DIR'/'$REPETITION
        NUM_OF_ROUTES=`wc -l routes.csv | cut -d " " -f 1`
        python2.7 count_jumps.py -a $ALPHA -b $BETA -e $EXPERIMENT -r $NUM_OF_ROUTES
        JUMPS=`cut -d ',' -f 6 route_jumps.csv`
        SUM=$((SUM + JUMPS))
        rm count_jumps.py
        cd ../..
	  done;
      # The value of '10' denotes the number of repetitions
      MEAN=$(echo "scale=2; $SUM / 10" | bc)
      RESULT=$EXPERIMENT","$ALPHA","$BETA","$MEAN
      echo $RESULT >> mean_jumps.csv

    done;
  done;
done;

