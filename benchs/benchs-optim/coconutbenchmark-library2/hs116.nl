g3 2 1 0	# problem hs116
 13 15 1 0 0	# vars, constraints, objectives, ranges, eqns
 10 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 10 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 46 3	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#c5
o2	#*
o2	#*
n1.231059
v2	#x[3]
v9	#x[10]
C1	#c6
o1	# - 
o2	#*
n0.00975
o5	#^
v1	#x[2]
n2
o2	#*
o2	#*
n0.975
v1	#x[2]
v4	#x[5]
C2	#c7
o1	# - 
o2	#*
n0.00975
o5	#^
v2	#x[3]
n2
o2	#*
o2	#*
n0.975
v2	#x[3]
v5	#x[6]
C3	#c8
o1	# - 
o2	#*
n0.00975
o5	#^
v0	#x[1]
n2
o2	#*
o2	#*
n0.975
v0	#x[1]
v3	#x[4]
C4	#c9
o2	#*
o2	#*
n1.231059
v1	#x[2]
v8	#x[9]
C5	#c10
o2	#*
o2	#*
n1.231059
v0	#x[1]
v7	#x[8]
C6	#c11
o54	#sumlist
4
o2	#*
v4	#x[5]
v6	#x[7]
o16	#-
o2	#*
v0	#x[1]
v7	#x[8]
o16	#-
o2	#*
v3	#x[4]
v6	#x[7]
o2	#*
v3	#x[4]
v7	#x[8]
C7	#c12
o54	#sumlist
4
o2	#*
n-0.002
o2	#*
v1	#x[2]
v8	#x[9]
o2	#*
n-0.002
o2	#*
v4	#x[5]
v7	#x[8]
o2	#*
n0.002
o2	#*
v0	#x[1]
v7	#x[8]
o2	#*
n0.002
o2	#*
v5	#x[6]
v8	#x[9]
C8	#c13
o54	#sumlist
4
o2	#*
v1	#x[2]
v8	#x[9]
o16	#-
o2	#*
v2	#x[3]
v9	#x[10]
o16	#-
o2	#*
v5	#x[6]
v8	#x[9]
o2	#*
v1	#x[2]
v9	#x[10]
C9	#c14
o0	# + 
o2	#*
n-0.002
o2	#*
v1	#x[2]
v9	#x[10]
o2	#*
n0.002
o2	#*
v2	#x[3]
v9	#x[10]
C10	#c1
n0
C11	#c2
n0
C12	#c3
n0
C13	#c4
n0
C14	#c15
n0
O0 0	#obj
n0
x13	# initial guess
0 0.5
1 0.8
2 0.9
3 0.1
4 0.14
5 0.5
6 489
7 80
8 650
9 450
10 150
11 150
12 150
r	#15 ranges (rhs's)
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 -1
2 0
2 0.9
2 0
2 0
2 -1
2 50
1 250
b	#13 bounds (on variables)
0 0.1 1
0 0.1 1
0 0.1 1
0 0.0001 0.1
0 0.1 0.9
0 0.1 0.9
0 0.1 1000
0 0.1 1000
0 500 1000
0 0.1 500
0 1 150
0 0.0001 150
0 0.0001 150
k12	#intermediate Jacobian column lengths
5
12
17
19
22
25
27
31
34
37
40
43
J0 3
2 0
9 -1.262626
12 1
J1 2
1 -0.03475
4 1
J2 2
2 -0.03475
5 1
J3 2
0 -0.03475
3 1
J4 3
1 0
8 -1.262626
11 1
J5 3
0 0
7 -1.262626
10 1
J6 5
0 0
3 0
4 0
6 0
7 0
J7 6
0 0
1 0
4 -1
5 -1
7 0
8 0
J8 5
1 -500
2 0
5 500
8 0
9 0
J9 3
1 1
2 0
9 0
J10 2
1 -1
2 1
J11 2
0 -1
1 1
J12 2
6 -0.002
7 0.002
J13 3
10 1
11 1
12 1
J14 3
10 1
11 1
12 1
G0 3
10 1
11 1
12 1
