g9 0 1 0 2 20130207 0 4 0 240	# problem polak2
 11 2 1 0 0	# vars, constraints, objectives, ranges, eqns
 2 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 10 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 22 1	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
o44	#exp
o54	#sumlist
10
o2	#*
n1e-08
o5	#^
v0	#x[1]
n2
o5	#^
o0	# + 
n2
v1	#x[2]
n2
o5	#^
v2	#x[3]
n2
o2	#*
n4
o5	#^
v3	#x[4]
n2
o5	#^
v4	#x[5]
n2
o5	#^
v5	#x[6]
n2
o5	#^
v6	#x[7]
n2
o5	#^
v7	#x[8]
n2
o5	#^
v8	#x[9]
n2
o5	#^
v9	#x[10]
n2
C1	#cons2
o44	#exp
o54	#sumlist
10
o2	#*
n1e-08
o5	#^
v0	#x[1]
n2
o5	#^
o0	# + 
n-2
v1	#x[2]
n2
o5	#^
v2	#x[3]
n2
o2	#*
n4
o5	#^
v3	#x[4]
n2
o5	#^
v4	#x[5]
n2
o5	#^
v5	#x[6]
n2
o5	#^
v6	#x[7]
n2
o5	#^
v7	#x[8]
n2
o5	#^
v8	#x[9]
n2
o5	#^
v9	#x[10]
n2
O0 0	#f
n0
x10	# initial guess
0 100
1 0.1
2 0.1
3 0.1
4 0.1
5 0.1
6 0.1
7 0.1
8 0.1
9 0.1
r	#2 ranges (rhs's)
1 0
1 0
b	#11 bounds (on variables)
3
3
3
3
3
3
3
3
3
3
3
k10	#intermediate Jacobian column lengths
2
4
6
8
10
12
14
16
18
20
J0 11
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
10 -1
J1 11
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
10 -1
G0 1
10 1
