g3 0 1 0	# problem hs056
 7 4 1 0 4	# vars, constraints, objectives, ranges, eqns
 4 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 4 7 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 10 3	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#constr1
o2	#*
n-4.2
o5	#^
o41	#sin
v0	#x[4]
n2
C1	#constr2
o2	#*
n-4.2
o5	#^
o41	#sin
v1	#x[5]
n2
C2	#constr3
o2	#*
n-4.2
o5	#^
o41	#sin
v2	#x[6]
n2
C3	#constr4
o2	#*
n-7.2
o5	#^
o41	#sin
v3	#x[7]
n2
O0 0	#obj
o16	#-
o2	#*
o2	#*
v4	#x[1]
v5	#x[2]
v6	#x[3]
x7	# initial guess
0 0.509739678831507
1 0.509739678831507
2 0.509739678831507
3 0.9851107833377457
4 1
5 1
6 1
r	#4 ranges (rhs's)
4 0
4 0
4 0
4 0
b	#7 bounds (on variables)
0 0 3.14159
0 0 3.14159
0 0 3.14159
0 0 3.14159
0 0 1.e8
0 0 1.e8
0 0 1.e8
k6	#intermediate Jacobian column lengths
1
2
3
4
6
8
J0 2
0 0
4 1
J1 2
1 0
5 1
J2 2
2 0
6 1
J3 4
3 0
4 1
5 2
6 2
G0 3
4 0
5 0
6 0
