g3 0 1 0	# problem hs093
 6 2 1 0 0	# vars, constraints, objectives, ranges, eqns
 2 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 6 6 6	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 12 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#constr1
o2	#*
n0.001
o2	#*
o2	#*
o2	#*
o2	#*
o2	#*
v0	#x[1]
v1	#x[2]
v2	#x[3]
v3	#x[4]
v4	#x[5]
v5	#x[6]
C1	#constr2
o0	# + 
o2	#*
o2	#*
o2	#*
o2	#*
n0.00062
v0	#x[1]
v3	#x[4]
o5	#^
v4	#x[5]
n2
o54	#sumlist
3
v0	#x[1]
v1	#x[2]
v2	#x[3]
o2	#*
o2	#*
o2	#*
o2	#*
n0.00058
v1	#x[2]
v2	#x[3]
o5	#^
v5	#x[6]
n2
o54	#sumlist
3
v0	#x[1]
o2	#*
n1.57
v1	#x[2]
v3	#x[4]
O0 0	#obj
o54	#sumlist
4
o2	#*
o2	#*
o2	#*
n0.0204
v0	#x[1]
v3	#x[4]
o54	#sumlist
3
v0	#x[1]
v1	#x[2]
v2	#x[3]
o2	#*
o2	#*
o2	#*
n0.0187
v1	#x[2]
v2	#x[3]
o54	#sumlist
3
v0	#x[1]
o2	#*
n1.57
v1	#x[2]
v3	#x[4]
o2	#*
o2	#*
o2	#*
o2	#*
n0.0607
v0	#x[1]
v3	#x[4]
o5	#^
v4	#x[5]
n2
o54	#sumlist
3
v0	#x[1]
v1	#x[2]
v2	#x[3]
o2	#*
o2	#*
o2	#*
o2	#*
n0.0437
v1	#x[2]
v2	#x[3]
o5	#^
v5	#x[6]
n2
o54	#sumlist
3
v0	#x[1]
o2	#*
n1.57
v1	#x[2]
v3	#x[4]
x6	# initial guess
0 5.54
1 4.4
2 12.02
3 11.82
4 0.702
5 0.852
r	#2 ranges (rhs's)
2 2.07
1 1
b	#6 bounds (on variables)
0 0  1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
k5	#intermediate Jacobian column lengths
2
4
6
8
10
J0 6
0 0
1 0
2 0
3 0
4 0
5 0
J1 6
0 0
1 0
2 0
3 0
4 0
5 0
G0 6
0 0
1 0
2 0
3 0
4 0
5 0
