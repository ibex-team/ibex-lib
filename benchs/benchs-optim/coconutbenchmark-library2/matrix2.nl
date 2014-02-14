g3 0 1 0	# problem matrix2
 6 2 1 0 0	# vars, constraints, objectives, ranges, eqns
 2 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 6 6 6	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 6 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
o1	# - 
o2	#*
v0	#x11
v2	#x22
o5	#^
v1	#x12
n2
C1	#cons2
o1	# - 
o2	#*
v3	#y11
v5	#y22
o5	#^
v4	#y12
n2
O0 0	#f
o54	#sumlist
3
o5	#^
o1	# - 
v0	#x11
v3	#y11
n2
o2	#*
n2
o5	#^
o1	# - 
v1	#x12
v4	#y12
n2
o5	#^
o1	# - 
v2	#x22
v5	#y22
n2
x6	# initial guess
0 1
1 1
2 1
3 1
4 1
5 1
r	#2 ranges (rhs's)
2 0
1 0
b	#6 bounds (on variables)
2 0
3
2 0
1 0
3
1 0
k5	#intermediate Jacobian column lengths
1
2
3
4
5
J0 3
0 0
1 0
2 0
J1 3
3 0
4 0
5 0
G0 6
0 0
1 0
2 0
3 0
4 0
5 0
