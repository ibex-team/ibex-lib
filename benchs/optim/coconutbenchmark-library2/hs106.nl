g3 4 1 0	# problem hs106
 8 6 1 0 0	# vars, constraints, objectives, ranges, eqns
 3 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 8 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 17 3	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#c4
o2	#*
v0	#x[1]
v5	#x[6]
C1	#c5
o1	# - 
o2	#*
v1	#x[2]
v6	#x[7]
o2	#*
v1	#x[2]
v3	#x[4]
C2	#c6
o1	# - 
o2	#*
v2	#x[3]
v7	#x[8]
o2	#*
v2	#x[3]
v4	#x[5]
C3	#c1
n0
C4	#c2
n0
C5	#c3
n0
O0 0	#obj
n0
x8	# initial guess
0 5000
1 5000
2 5000
3 200
4 350
5 150
6 225
7 425
r	#6 ranges (rhs's)
2 -83333.33
2 0
2 1250000
2 -1
2 -1
2 -1
b	#8 bounds (on variables)
0 100 10000
0 1000 10000
0 1000 10000
0 10 1000
0 10 1000
0 10 1000
0 10 1000
0 10 1000
k7	#intermediate Jacobian column lengths
1
2
3
7
11
13
15
J0 3
0 -100
3 -833.3325
5 0
J1 4
1 0
3 1250
4 -1250
6 0
J2 3
2 0
4 2500
7 0
J3 2
3 -0.0025
5 -0.0025
J4 3
3 0.0025
4 -0.0025
6 -0.0025
J5 2
4 0.01
7 -0.01
G0 3
0 1
1 1
2 1
