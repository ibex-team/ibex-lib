g3 1 1 0	# problem s365mod
 7 5 1 0 0	# vars, constraints, objectives, ranges, eqns
 5 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 7 2 2	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 22 2	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 2 0 0 0	# common exprs: b,c,o,c1,o1
V7 0 0	#P
o0	# + 
o39	#sqrt
o5	#^
v2	#x[2]
n2
o5	#^
v1	#x[3]
n2
V8 0 0	#Q
o0	# + 
o39	#sqrt
o5	#^
v1	#x[3]
n2
o5	#^
o1	# - 
v2	#x[2]
v0	#x[1]
n2
C0	#cons1
o0	# + 
o5	#^
o1	# - 
v3	#x[4]
v5	#x[6]
n2
o5	#^
o1	# - 
v4	#x[5]
v6	#x[7]
n2
C1	#cons2
o3	#/
o1	# - 
o2	#*
v1	#x[3]
v3	#x[4]
o2	#*
v2	#x[2]
v4	#x[5]
v7	#P
C2	#cons3
o3	#/
o1	# - 
o2	#*
v1	#x[3]
v5	#x[6]
o2	#*
v2	#x[2]
v6	#x[7]
v7	#P
C3	#cons4
o3	#/
o1	# - 
o0	# + 
o2	#*
v0	#x[1]
v1	#x[3]
o2	#*
o1	# - 
v2	#x[2]
v0	#x[1]
v4	#x[5]
o2	#*
v1	#x[3]
v3	#x[4]
v8	#Q
C4	#cons5
o3	#/
o1	# - 
o0	# + 
o2	#*
v0	#x[1]
v1	#x[3]
o2	#*
o1	# - 
v2	#x[2]
v0	#x[1]
v6	#x[7]
o2	#*
v1	#x[3]
v5	#x[6]
v8	#Q
O0 0	#f
o2	#*
v0	#x[1]
v1	#x[3]
x7	# initial guess
0 3
1 2
2 0.01
3 -1.5
4 1.5
5 5
6 0
r	#5 ranges (rhs's)
2 4
2 1
2 1
2 1
2 1
b	#7 bounds (on variables)
0 0.5 1.e8
0 0.5 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 1 1.e8
0 -1.e8 1.e8
0 1 1.e8
k6	#intermediate Jacobian column lengths
2
6
10
13
16
19
J0 4
3 0
4 0
5 0
6 0
J1 4
1 0
2 0
3 0
4 0
J2 4
1 0
2 0
5 0
6 0
J3 5
0 0
1 0
2 0
3 0
4 0
J4 5
0 0
1 0
2 0
5 0
6 0
G0 2
0 0
1 0
