g3 0 1 0	# problem odfits
 10 6 1 0 6	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 10 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 15 10	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1[1]
n0
C1	#cons1[2]
n0
C2	#cons1[3]
n0
C3	#cons1[4]
n0
C4	#cons1[5]
n0
C5	#cons1[6]
n0
O0 0	#f
o54	#sumlist
10
o2	#*
n0.5
o2	#*
v0	#T[13]
o43	#log
o3	#/
v0	#T[13]
n90
o2	#*
n0.5
o2	#*
v1	#T[14]
o43	#log
o3	#/
v1	#T[14]
n450
o2	#*
n0.5
o2	#*
v2	#T[23]
o43	#log
o3	#/
v2	#T[23]
n360
o2	#*
n0.2
o2	#*
v3	#T[24]
o43	#log
v3	#T[24]
o2	#*
n0.6666666666666666
o2	#*
v4	#F[1]
o43	#log
o3	#/
v4	#F[1]
n100
o2	#*
n0.6666666666666666
o2	#*
v5	#F[2]
o43	#log
o3	#/
v5	#F[2]
n500
o2	#*
n0.6666666666666666
o2	#*
v6	#F[3]
o43	#log
o3	#/
v6	#F[3]
n400
o2	#*
n0.6666666666666666
o2	#*
v7	#F[4]
o43	#log
o3	#/
v7	#F[4]
n1100
o2	#*
n0.6666666666666666
o2	#*
v8	#F[5]
o43	#log
o3	#/
v8	#F[5]
n600
o2	#*
n0.6666666666666666
o2	#*
v9	#F[6]
o43	#log
o3	#/
v9	#F[6]
n700
x10	# initial guess
0 90
1 450
2 360
3 1
4 100
5 500
6 400
7 1100
8 600
9 700
r	#6 ranges (rhs's)
4 0
4 0
4 0
4 0
4 0
4 0
b	#10 bounds (on variables)
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
0 0.1 1.e8
k9	#intermediate Jacobian column lengths
1
3
6
9
10
11
12
13
14
J0 2
0 1
4 -1
J1 2
1 1
5 -1
J2 2
2 1
6 -1
J3 4
1 1
2 1
3 1
7 -1
J4 3
2 1
3 1
8 -1
J5 2
3 1
9 -1
G0 10
0 -0.5
1 -0.5
2 -0.5
3 -0.2
4 -0.6666666666666666
5 -0.6666666666666666
6 -0.6666666666666666
7 -0.6666666666666666
8 -0.6666666666666666
9 -0.6666666666666666
