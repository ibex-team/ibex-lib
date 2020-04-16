set term pdf enhanced

set output outputfile

set title title

set border 3

set logscale x 10
set format x "10^{%T}"
set xtics 1e1
set xtics add ("1" 1)
set xtics nomirror
set mxtics 5
set xrange [1:1e-8]
set xlabel "ε"

set yrange [0:]
set ytics nomirror
set ylabel "time (in sec)"

set style arrow 1 nohead lc rgb "gray" lt 0 lw 2
prec_max = 10**-prec_ndigits_max
prec_min = 10**-prec_ndigits_min
set arrow from prec_max, graph 0 to prec_max, graph 1 as 1
set arrow from prec_min, graph 0 to prec_min, graph 1 as 1
set arrow from graph 0, time_limit to graph 1, time_limit as 1

plot datafile using "eps":"time" with points ps 0.5 lc rgb "red" notitle,\
     datafile using "eps":"time" smooth unique lc rgb "orange" notitle
