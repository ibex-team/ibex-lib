g3 0 1 0	# problem hs097
 6 4 1 0 0	# vars, constraints, objectives, ranges, eqns
 4 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 5 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 20 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#constr1
o16	#-
o54	#sumlist
5
o2	#*
o2	#*
n169
v0	#x[1]
v1	#x[3]
o2	#*
o2	#*
n3580
v1	#x[3]
v3	#x[5]
o2	#*
o2	#*
n3810
v2	#x[4]
v3	#x[5]
o2	#*
o2	#*
n18500
v2	#x[4]
v4	#x[6]
o2	#*
o2	#*
n24300
v3	#x[5]
v4	#x[6]
C1	#constr2
o16	#-
o54	#sumlist
4
o2	#*
o2	#*
n139
v0	#x[1]
v1	#x[3]
o2	#*
o2	#*
n2450
v2	#x[4]
v3	#x[5]
o2	#*
o2	#*
n16600
v2	#x[4]
v4	#x[6]
o2	#*
o2	#*
n17200
v3	#x[5]
v4	#x[6]
C2	#constr3
o2	#*
o2	#*
n26000
v2	#x[4]
v3	#x[5]
C3	#constr4
o16	#-
o2	#*
o2	#*
n14000
v0	#x[1]
v4	#x[6]
O0 0	#obj
n0
x6	# initial guess
0 0
1 0
2 0
3 0
4 0
5 0
r	#4 ranges (rhs's)
2 32.97
2 25.12
2 -29.08
2 -78.02
b	#6 bounds (on variables)
0 0 0.31
0 0 0.068
0 0 0.042
0 0 0.028
0 0 0.0134
0 0 0.046
k5	#intermediate Jacobian column lengths
3
5
9
13
16
J0 6
0 17.1
1 204.2
2 212.3
3 623.4
4 1495.5
5 38.2
J1 6
0 17.9
1 113.9
2 169.7
3 337.8
4 1385.2
5 36.8
J2 3
2 -70
3 -819
5 -273
J3 5
0 159.9
2 587
3 391
4 2198
5 -311
G0 6
0 4.3
1 63.3
2 15.8
3 68.5
4 4.7
5 31.8
