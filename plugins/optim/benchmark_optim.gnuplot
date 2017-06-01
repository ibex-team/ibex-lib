set term pdf enhanced

set output outputfile

set title title

set border 3

set logscale x 10
set format x "10^{%T}"
set xtics 1e1
set xtics add ("1" 1)
set xtics nomirror
set xrange [1:1e-10]
set xlabel "ε"

set yrange [0:]
set ytics nomirror
set ylabel "time (in sec)"

plot datafile using 1:2 with lp ps 0.5 lc rgb "red" notitle
