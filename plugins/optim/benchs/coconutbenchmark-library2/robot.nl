g3 1 1 0	# problem robot
 7 2 1 0 2	# vars, constraints, objectives, ranges, eqns
 2 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 7 7 7	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 14 7	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
o54	#sumlist
7
o46	#cos
v0	#TH[1]
o46	#cos
v1	#TH[2]
o46	#cos
v2	#TH[3]
o46	#cos
v3	#TH[4]
o46	#cos
v4	#TH[5]
o46	#cos
v5	#TH[6]
o2	#*
n0.5
o46	#cos
v6	#TH[7]
C1	#cons2
o54	#sumlist
7
o41	#sin
v0	#TH[1]
o41	#sin
v1	#TH[2]
o41	#sin
v2	#TH[3]
o41	#sin
v3	#TH[4]
o41	#sin
v4	#TH[5]
o41	#sin
v5	#TH[6]
o2	#*
n0.5
o41	#sin
v6	#TH[7]
O0 0	#f
o54	#sumlist
7
o5	#^
v0	#TH[1]
n2
o5	#^
v1	#TH[2]
n2
o5	#^
v2	#TH[3]
n2
o5	#^
v3	#TH[4]
n2
o5	#^
v4	#TH[5]
n2
o5	#^
v5	#TH[6]
n2
o5	#^
v6	#TH[7]
n2
r	#2 ranges (rhs's)
4 4
4 4
b	#7 bounds (on variables)
3
3
3
3
3
3
3
k6	#intermediate Jacobian column lengths
2
4
6
8
10
12
J0 7
0 0
1 0
2 0
3 0
4 0
5 0
6 0
J1 7
0 0
1 0
2 0
3 0
4 0
5 0
6 0
G0 7
0 0
1 0
2 0
3 0
4 0
5 0
6 0
