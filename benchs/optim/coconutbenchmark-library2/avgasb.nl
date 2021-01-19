g3 2 1 0	# problem avgasb
 8 10 1 0 0	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 8 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 30 8	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#con1[1]
n0
C1	#con1[2]
n0
C2	#con1[3]
n0
C3	#con1[4]
n0
C4	#con5[0]
n0
C5	#con5[1]
n0
C6	#con7
n0
C7	#con8
n0
C8	#con9
n0
C9	#con10
n0
O0 0	#obj
o54	#sumlist
15
o2	#*
n2
o5	#^
v0	#x[1]
n2
o2	#*
n2
o5	#^
v1	#x[2]
n2
o2	#*
n2
o5	#^
v2	#x[3]
n2
o2	#*
n2
o5	#^
v3	#x[4]
n2
o2	#*
n2
o5	#^
v4	#x[5]
n2
o2	#*
n2
o5	#^
v5	#x[6]
n2
o2	#*
n2
o5	#^
v6	#x[7]
n2
o2	#*
n2
o5	#^
v7	#x[8]
n2
o16	#-
o2	#*
v0	#x[1]
v1	#x[2]
o16	#-
o2	#*
v1	#x[2]
v2	#x[3]
o16	#-
o2	#*
v2	#x[3]
v3	#x[4]
o16	#-
o2	#*
v3	#x[4]
v4	#x[5]
o16	#-
o2	#*
v4	#x[5]
v5	#x[6]
o16	#-
o2	#*
v5	#x[6]
v6	#x[7]
o16	#-
o2	#*
v6	#x[7]
v7	#x[8]
x8	# initial guess
0 0.5
1 0.5
2 0.5
3 0.5
4 0.5
5 0.5
6 0.5
7 0.5
r	#10 ranges (rhs's)
1 1
1 1
1 1
1 1
1 2
1 2
1 0
1 0
1 0
1 0
b	#8 bounds (on variables)
0 0 1
0 0 1
0 0 1
0 0 1
0 0 1
0 0 1
0 0 1
0 0 1
k7	#intermediate Jacobian column lengths
4
8
12
15
18
22
26
J0 2
0 1
1 1
J1 2
2 1
3 1
J2 2
4 1
5 1
J3 2
6 1
7 1
J4 4
1 1
3 1
5 1
7 1
J5 4
0 1
2 1
4 1
6 1
J6 3
0 -2
2 -1
6 1
J7 4
0 -5
2 -3
4 3
6 1
J8 4
1 -1
3 1
5 3
7 5
J9 3
1 -1
5 3
7 2
G0 8
0 0
1 -2
2 -1
3 -3
4 -2
5 -4
6 -3
7 -5
