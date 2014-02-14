g3 0 1 0	# problem dipigri
 7 4 1 0 0	# vars, constraints, objectives, ranges, eqns
 4 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 5 7 5	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 19 7	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
o54	#sumlist
3
o2	#*
n2
o5	#^
v0	#x[1]
n2
o2	#*
n3
o5	#^
v1	#x[2]
n4
o2	#*
n4
o5	#^
v3	#x[4]
n2
C1	#cons2
o2	#*
n10
o5	#^
v2	#x[3]
n2
C2	#cons3
o0	# + 
o5	#^
v1	#x[2]
n2
o2	#*
n6
o5	#^
v4	#x[6]
n2
C3	#cons4
o54	#sumlist
4
o2	#*
n4
o5	#^
v0	#x[1]
n2
o5	#^
v1	#x[2]
n2
o16	#-
o2	#*
o2	#*
n3
v0	#x[1]
v1	#x[2]
o2	#*
n2
o5	#^
v2	#x[3]
n2
O0 0	#f
o54	#sumlist
8
o5	#^
o0	# + 
n-10
v0	#x[1]
n2
o2	#*
n5
o5	#^
o0	# + 
n-12
v1	#x[2]
n2
o5	#^
v2	#x[3]
n4
o2	#*
n3
o5	#^
o0	# + 
n-11
v3	#x[4]
n2
o2	#*
n10
o5	#^
v5	#x[5]
n6
o2	#*
n7
o5	#^
v4	#x[6]
n2
o5	#^
v6	#x[7]
n4
o16	#-
o2	#*
o2	#*
n4
v4	#x[6]
v6	#x[7]
x7	# initial guess
0 1
1 2
2 0
3 4
4 1
5 0
6 1
r	#4 ranges (rhs's)
1 127
1 282
1 196
1 0
b	#7 bounds (on variables)
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k6	#intermediate Jacobian column lengths
4
8
11
13
15
17
J0 5
0 0
1 0
2 1
3 0
5 5
J1 5
0 7
1 3
2 0
3 1
5 -1
J2 4
0 23
1 0
4 0
6 -8
J3 5
0 0
1 0
2 0
4 5
6 -11
G0 7
0 0
1 0
2 0
3 0
4 -10
5 0
6 -8
