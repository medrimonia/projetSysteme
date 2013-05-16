#!/bin/bash

INPUT="tempsExecution"
OUTPUT="compare"

gnuplot -persist <<PLOT

set title "Time used depending on number of kernel threads"

set xrange [1:8]
set yrange [0:*]

set xlabel "Number of kernel threads"

set ylabel "Time"

#set datafile separator ";"

set tics out

#In case for building an eps-file ...
#set terminal postscript enhanced color solid eps 15
#set output "${OUTPUT}.eps"

#In case for building a png-file ...
set terminal png size 1024,768
set output "${OUTPUT}.png"

plot '${INPUT}' using 1:2 with lines lw 2 title "no thread time",\
     '${INPUT}' using 1:3 with lines lw 2 title "pthread time",\
     '${INPUT}' using 1:4 with lines lw 2 title "custom thread time"
quit
PLOT