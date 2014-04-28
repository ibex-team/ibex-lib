g3 0 1 0	# problem genhs28
 10 8 1 0 8	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 10 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 24 10	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons[1]
n0
C1	#cons[2]
n0
C2	#cons[3]
n0
C3	#cons[4]
n0
C4	#cons[5]
n0
C5	#cons[6]
n0
C6	#cons[7]
n0
C7	#cons[8]
n0
O0 0	#f
o54	#sumlist
9
o5	#^
o0	# + 
v0	#x[1]
v1	#x[2]
n2
o5	#^
o0	# + 
v1	#x[2]
v2	#x[3]
n2
o5	#^
o0	# + 
v2	#x[3]
v3	#x[4]
n2
o5	#^
o0	# + 
v3	#x[4]
v4	#x[5]
n2
o5	#^
o0	# + 
v4	#x[5]
v5	#x[6]
n2
o5	#^
o0	# + 
v5	#x[6]
v6	#x[7]
n2
o5	#^
o0	# + 
v6	#x[7]
v7	#x[8]
n2
o5	#^
o0	# + 
v7	#x[8]
v8	#x[9]
n2
o5	#^
o0	# + 
v8	#x[9]
v9	#x[10]
n2
x10	# initial guess
0 -4
1 1
2 1
3 1
4 1
5 1
6 1
7 1
8 1
9 1
r	#8 ranges (rhs's)
4 1
4 1
4 1
4 1
4 1
4 1
4 1
4 1
b	#10 bounds (on variables)
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k9	#intermediate Jacobian column lengths
1
3
6
9
12
15
18
21
23
J0 3
0 1
1 2
2 3
J1 3
1 1
2 2
3 3
J2 3
2 1
3 2
4 3
J3 3
3 1
4 2
5 3
J4 3
4 1
5 2
6 3
J5 3
5 1
6 2
7 3
J6 3
6 1
7 2
8 3
J7 3
7 1
8 2
9 3
G0 10
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
9 0
