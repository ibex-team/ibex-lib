g3 0 1 0	# problem polak4
 3 3 1 0 0	# vars, constraints, objectives, ranges, eqns
 3 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 2 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 9 1	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
o0	# + 
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
C1	#cons2
o0	# + 
o2	#*
n0.01
o5	#^
v0	#x[1]
n2
o2	#*
n0.01
o5	#^
v1	#x[2]
n2
C2	#cons3
o0	# + 
o2	#*
n1e+05
o5	#^
o0	# + 
n-2
v0	#x[1]
n2
o5	#^
v1	#x[2]
n2
O0 0	#f
n0
x2	# initial guess
0 0.9
1 0.1
r	#3 ranges (rhs's)
1 1
1 0.01
1 1e+05
b	#3 bounds (on variables)
3
3
3
k2	#intermediate Jacobian column lengths
3
6
J0 3
0 -1
1 0
2 -1
J1 3
0 0
1 0
2 -1
J2 3
0 0
1 0
2 -1
G0 1
2 1
