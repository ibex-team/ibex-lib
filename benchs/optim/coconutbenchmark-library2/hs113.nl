g3 0 1 0	# problem hs113
 10 8 1 0 0	# vars, constraints, objectives, ranges, eqns
 5 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 5 10 5	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 32 10	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#c4
o54	#sumlist
3
o2	#*
n-3
o5	#^
o0	# + 
n-2
v0	#x[1]
n2
o2	#*
n-4
o5	#^
o0	# + 
n-3
v1	#x[2]
n2
o2	#*
n-2
o5	#^
v2	#x[3]
n2
C1	#c5
o1	# - 
o2	#*
n-5
o5	#^
v0	#x[1]
n2
o5	#^
o0	# + 
n-6
v2	#x[3]
n2
C2	#c6
o54	#sumlist
3
o2	#*
n-0.5
o5	#^
o0	# + 
n-8
v0	#x[1]
n2
o2	#*
n-2
o5	#^
o0	# + 
n-4
v1	#x[2]
n2
o2	#*
n-3
o5	#^
v3	#x[5]
n2
C3	#c7
o54	#sumlist
3
o16	#-
o5	#^
v0	#x[1]
n2
o2	#*
n-2
o5	#^
o0	# + 
n-2
v1	#x[2]
n2
o2	#*
o2	#*
n2
v0	#x[1]
v1	#x[2]
C4	#c8
o2	#*
n-12
o5	#^
o0	# + 
n-8
v4	#x[9]
n2
C5	#c1
n0
C6	#c2
n0
C7	#c3
n0
O0 0	#obj
o54	#sumlist
12
o5	#^
v0	#x[1]
n2
o5	#^
v1	#x[2]
n2
o2	#*
v0	#x[1]
v1	#x[2]
o5	#^
o0	# + 
n-10
v2	#x[3]
n2
o2	#*
n4
o5	#^
o0	# + 
n-5
v5	#x[4]
n2
o5	#^
o0	# + 
n-3
v3	#x[5]
n2
o2	#*
n2
o5	#^
o0	# + 
n-1
v6	#x[6]
n2
o2	#*
n5
o5	#^
v7	#x[7]
n2
o2	#*
n7
o5	#^
o0	# + 
n-11
v8	#x[8]
n2
o2	#*
n2
o5	#^
o0	# + 
n-10
v4	#x[9]
n2
o5	#^
o0	# + 
n-7
v9	#x[10]
n2
n45
x10	# initial guess
0 2
1 3
2 5
3 1
4 6
5 5
6 2
7 7
8 3
9 10
r	#8 ranges (rhs's)
2 -120
2 -40
2 -30
2 0
2 0
2 -105
2 0
2 -12
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
8
16
18
20
22
24
26
28
30
J0 4
0 0
1 0
2 0
5 7
J1 4
0 0
1 -8
2 0
5 2
J2 4
0 0
1 0
3 0
6 1
J3 4
0 0
1 0
3 -14
6 6
J4 4
0 3
1 -6
4 0
9 7
J5 4
0 -4
1 -5
7 3
8 -9
J6 4
0 -10
1 8
7 17
8 -2
J7 4
0 8
1 -2
4 -5
9 2
G0 10
0 -14
1 -16
2 0
3 0
4 0
5 0
6 0
7 0
8 0
9 0
