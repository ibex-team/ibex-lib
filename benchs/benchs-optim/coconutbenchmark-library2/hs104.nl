g3 0 1 0	# problem hs104
 8 6 1 0 0	# vars, constraints, objectives, ranges, eqns
 6 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 8 4 4	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 21 4	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#c1
o16	#-
o2	#*
o2	#*
n0.0588
v6	#x[5]
v2	#x[7]
C1	#c2
o16	#-
o2	#*
o2	#*
n0.0588
v7	#x[6]
v3	#x[8]
C2	#c3
o16	#-
o54	#sumlist
3
o3	#/
o2	#*
n4
v4	#x[3]
v6	#x[5]
o3	#/
n2
o2	#*
o5	#^
v4	#x[3]
n0.71
v6	#x[5]
o3	#/
o2	#*
n0.0588
v2	#x[7]
o5	#^
v4	#x[3]
n1.3
C3	#c4
o16	#-
o54	#sumlist
3
o3	#/
o2	#*
n4
v5	#x[4]
v7	#x[6]
o3	#/
n2
o2	#*
o5	#^
v5	#x[4]
n0.71
v7	#x[6]
o3	#/
o2	#*
n0.0588
v3	#x[8]
o5	#^
v5	#x[4]
n1.3
C4	#c5
o0	# + 
o2	#*
o2	#*
n0.4
o5	#^
v0	#x[1]
n0.67
o5	#^
v2	#x[7]
n-0.67
o2	#*
o2	#*
n0.4
o5	#^
v1	#x[2]
n0.67
o5	#^
v3	#x[8]
n-0.67
C5	#c6
o0	# + 
o2	#*
o2	#*
n0.4
o5	#^
v0	#x[1]
n0.67
o5	#^
v2	#x[7]
n-0.67
o2	#*
o2	#*
n0.4
o5	#^
v1	#x[2]
n0.67
o5	#^
v3	#x[8]
n-0.67
O0 0	#obj
o54	#sumlist
3
o2	#*
o2	#*
n0.4
o5	#^
v0	#x[1]
n0.67
o5	#^
v2	#x[7]
n-0.67
o2	#*
o2	#*
n0.4
o5	#^
v1	#x[2]
n0.67
o5	#^
v3	#x[8]
n-0.67
n10
x8	# initial guess
0 6
1 3
2 1
3 0.5
4 0.4
5 0.2
6 6
7 6
r	#6 ranges (rhs's)
2 -1
2 -1
2 -1
2 -1
2 -9.9
1 -5.8
b	#8 bounds (on variables)
0 0.1 10
0 0.1 10
0 0.1 10
0 0.1 10
0 0.1 10
0 0.1 10
0 0.1 10
0 0.1 10
k7	#intermediate Jacobian column lengths
4
7
11
15
16
17
19
J0 3
0 -0.1
2 0
6 0
J1 4
0 -0.1
1 -0.1
3 0
7 0
J2 3
2 0
4 0
6 0
J3 3
3 0
5 0
7 0
J4 4
0 -1
1 -1
2 0
3 0
J5 4
0 -1
1 -1
2 0
3 0
G0 4
0 -1
1 -1
2 0
3 0
