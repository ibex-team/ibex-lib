set term pdf enhanced

set output outputfile

set title "Scatter plot between current benchmarks and results from ".cmp_ref

set border 3

set size square

set xtics nomirror
set mxtics 2
set xrange [0:time_limit]
set xlabel "Current benchmarks"

set ytics nomirror
set mytics 2
set yrange [0:time_limit]
set ylabel "Previous benchmarks"

set style arrow 1 nohead lc rgb "gray"
set arrow from graph 0, graph 0 to graph 1, graph 1 as 1

plot datafile using "current":"previous" with points ps 0.5 lc rgb "red" notitle
