set term pdf enhanced

set output outputfile

set title "Scatter plot between results from ".ref0." and results from ".ref1

set border 3

set size square

set xtics nomirror
set mxtics 2
set xrange [0:time_limit]
set xlabel ref0

set ytics nomirror
set mytics 2
set yrange [0:time_limit]
set ylabel ref1

set style arrow 1 nohead lc rgb "gray"
set arrow from graph 0, graph 0 to graph 1, graph 1 as 1

plot datafile using 2:3 with points ps 0.5 lc rgb "red" notitle
