set term png size 400,250

set output outputfile

plot datafile using 1:2 with lp
