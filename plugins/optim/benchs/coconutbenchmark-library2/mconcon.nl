g3 8 1 0	# problem mconcon
 15 11 1 0 11	# vars, constraints, objectives, ranges, eqns
 4 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 11 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 26 7	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1_1
o54	#sumlist
3
o2	#*
v0	#P[1]
o15	#abs
v0	#P[1]
o16	#-
o2	#*
v1	#P[2]
o15	#abs
v1	#P[2]
o16	#-
o2	#*
o2	#*
n0.597053452
v7	#Q[1]
o5	#^
o15	#abs
v7	#Q[1]
n0.8539
C1	#cons1_2
o54	#sumlist
3
o2	#*
v2	#P[3]
o15	#abs
v2	#P[3]
o16	#-
o2	#*
v3	#P[4]
o15	#abs
v3	#P[4]
o16	#-
o2	#*
o2	#*
n0.597053452
v8	#Q[2]
o5	#^
o15	#abs
v8	#Q[2]
n0.8539
C2	#cons1_3
o54	#sumlist
3
o2	#*
v3	#P[4]
o15	#abs
v3	#P[4]
o16	#-
o2	#*
v4	#P[5]
o15	#abs
v4	#P[5]
o16	#-
o2	#*
o2	#*
n0.597053452
v9	#Q[3]
o5	#^
o15	#abs
v9	#Q[3]
n0.8539
C3	#cons1_4
o54	#sumlist
3
o2	#*
v5	#P[6]
o15	#abs
v5	#P[6]
o16	#-
o2	#*
v6	#P[7]
o15	#abs
v6	#P[7]
o16	#-
o2	#*
o2	#*
n0.597053452
v10	#Q[4]
o5	#^
o15	#abs
v10	#Q[4]
n0.8539
C4	#cons2
n0
C5	#cons3
n0
C6	#cons4
n0
C7	#cons5
n0
C8	#cons6
n0
C9	#cons7
n0
C10	#cons8
n0
O0 0	#f
n0
x15	# initial guess
0 965
1 965
2 965
3 965
4 965
5 965
6 965
7 100
8 100
9 -100
10 -100
11 1000
12 1000
13 1000
14 1000
r	#11 ranges (rhs's)
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 -1000
4 0
4 0
4 0
b	#15 bounds (on variables)
0 -1.e8 914.73
0 -1.e8 1.e8
0 -1.e8 904.73
0 -1.e8 1.e8
0 -1.e8  904.73
0 -1.e8 1.e8
0 -1.e8 914.73
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
1 400
k14	#intermediate Jacobian column lengths
1
2
3
5
6
7
8
11
14
17
20
22
24
25
J0 3
0 0
1 0
7 0
J1 3
2 0
3 0
8 0
J2 3
3 0
4 0
9 0
J3 3
5 0
6 0
10 0
J4 2
7 1
13 -1
J5 2
7 -1
11 1
J6 2
8 1
11 -1
J7 2
8 -1
9 1
J8 2
9 -1
12 -1
J9 2
10 1
12 1
J10 2
10 -1
14 -1
G0 7
0 -1
1 -1
2 -1
3 -1
4 -1
5 -1
6 -1
