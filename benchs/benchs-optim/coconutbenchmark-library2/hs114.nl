g9 4 1 0 11 20130207 0 4 0 4	# problem hs114
 10 11 1 0 3	# vars, constraints, objectives, ranges, eqns
 6 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 7 8 1	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 31 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 2 0 0 0	# common exprs: b,c,o,c1,o1
V10 0 0	#nl(G5)
o1	# - 
o2	#*
o2	#*
n0.13167
v1	#x[1]
v5	#x[8]
o2	#*
o2	#*
n0.00667
v1	#x[1]
o5	#^
v5	#x[8]
n2
V11 0 0	#nl(G6)
o2	#*
n-0.038
o5	#^
v5	#x[8]
n2
C0	#g5
v10	#nl(G5)
C1	#g6
v11	#nl(G6)
C2	#g7
o16	#-
v10	#nl(G5)
C3	#g8
o16	#-
v11	#nl(G6)
C4	#g10
o3	#/
o2	#*
n98000
v3	#x[3]
o0	# + 
o2	#*
v0	#x[4]
v6	#x[9]
o2	#*
n1000
v3	#x[3]
C5	#g11
o3	#/
o0	# + 
v2	#x[2]
v4	#x[5]
v1	#x[1]
C6	#g1
n0
C7	#g2
n0
C8	#g3
n0
C9	#g4
n0
C10	#g9
n0
O0 0	#obj
o16	#-
o2	#*
o2	#*
n0.063
v0	#x[4]
v7	#x[7]
x10	# initial guess
0 3048
1 1745
2 12000
3 110
4 1974
5 8
6 3.6
7 92.8
8 89.2
9 145
r	#11 ranges (rhs's)
2 0
2 -57.425
2 0
2 57.425
4 0
4 0
2 -35.82
2 133
2 35.82
2 -133
4 0
b	#10 bounds (on variables)
0 1e-05 5000
0 1e-05 2000
0 1e-05 16000
0 1e-05 120
0 1e-05 2000
0 3 12
0 1.2 4
0 90 95
0 85 93
0 145 162
k9	#intermediate Jacobian column lengths
4
8
9
10
12
17
20
24
27
J0 3
0 -0.99
1 1.12
5 0
J1 3
5 1.098
7 -0.99
8 0.325
J2 3
0 1.0101010101010102
1 -1.12
5 0
J3 3
5 -1.098
7 1.0101010101010102
8 -0.325
J4 4
0 0
3 0
6 0
8 -1
J5 4
1 0
2 0
4 0
5 -1
J6 2
6 -0.9
9 -0.222
J7 2
7 3
9 -0.99
J8 2
6 1.1111111111111112
9 0.222
J9 2
7 -3
9 1.0101010101010102
J10 3
0 1.22
1 -1
4 -1
G0 6
0 0
1 5.04
2 0.035
3 10
4 3.36
7 0
