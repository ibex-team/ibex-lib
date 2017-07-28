g9 2 1 0 6 20130207 0 4 0 240	# problem synthes1
 6 6 1 0 0	# vars, constraints, objectives, ranges, eqns
 2 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 2 2 2	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 16 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#n1
o0	# + 
o2	#*
n-0.8
o43	#log
o0	# + 
n1
v1	#x2
o2	#*
n-0.96
o43	#log
o0	# + 
n1
o1	# - 
v0	#x1
v1	#x2
C1	#n2
o1	# - 
o2	#*
n-1.2
o43	#log
o0	# + 
n1
o1	# - 
v0	#x1
v1	#x2
o43	#log
o0	# + 
n1
v1	#x2
C2	#l3
n0
C3	#l4
n0
C4	#l5
n0
C5	#l6
n0
O0 0	#obj
o54	#sumlist
3
o2	#*
n-18
o43	#log
o0	# + 
n1
v1	#x2
o2	#*
n-19.2
o43	#log
o0	# + 
n1
o1	# - 
v0	#x1
v1	#x2
n10
r	#6 ranges (rhs's)
1 0
1 2
2 0
2 0
2 0
2 -1
b	#6 bounds (on variables)
0 0 2
0 0 2
0 0 1
0 0 1
0 0 1
0 0 1
k5	#intermediate Jacobian column lengths
4
9
11
13
15
J0 3
0 0
1 0
2 0.8
J1 4
0 0
1 0
2 1
5 2
J2 2
0 1
1 -1
J3 2
1 -1
3 2
J4 3
0 -1
1 1
4 2
J5 2
3 -1
4 -1
G0 6
0 10
1 0
2 -7
3 5
4 6
5 8
