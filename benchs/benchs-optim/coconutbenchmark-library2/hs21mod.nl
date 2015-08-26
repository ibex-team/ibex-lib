g3 2 1 0	# problem hs21mod
 7 1 1 0 0	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 7 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 2 7	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
n0
O0 0	#f
o54	#sumlist
8
o2	#*
n0.01
o5	#^
v0	#x[1]
n2
o2	#*
n0.01
o5	#^
v2	#x[3]
n2
o2	#*
n0.01
o5	#^
v4	#x[5]
n2
o2	#*
n0.01
o5	#^
v5	#x[6]
n2
o5	#^
v1	#x[2]
n2
o5	#^
v3	#x[4]
n2
o5	#^
v6	#x[7]
n2
n-100
x7	# initial guess
0 -1
1 -1
2 0
3 0
4 0
5 0
6 0
r	#1 ranges (rhs's)
2 10
b	#7 bounds (on variables)
0 2 50
0 -50 50
1 50
2 2
3
1 0
2 0
k6	#intermediate Jacobian column lengths
1
2
2
2
2
2
J0 2
0 10
1 -1
G0 7
0 0
1 0
2 0
3 0
4 0
5 0
6 0
