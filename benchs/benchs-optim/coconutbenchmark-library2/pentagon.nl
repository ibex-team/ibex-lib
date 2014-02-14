g3 10 1 0	# problem pentagon
 6 12 1 0 0	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 6 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 24 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons[1,1]
n0
C1	#cons[1,2]
n0
C2	#cons[1,3]
n0
C3	#cons[1,4]
n0
C4	#cons[2,1]
n0
C5	#cons[2,2]
n0
C6	#cons[2,3]
n0
C7	#cons[2,4]
n0
C8	#cons[3,1]
n0
C9	#cons[3,2]
n0
C10	#cons[3,3]
n0
C11	#cons[3,4]
n0
O0 0	#f
o54	#sumlist
3
o3	#/
n1
o5	#^
o0	# + 
o5	#^
o1	# - 
v0	#x[1]
v1	#x[2]
n2
o5	#^
o1	# - 
v3	#y[1]
v4	#y[2]
n2
n8
o3	#/
n1
o5	#^
o0	# + 
o5	#^
o1	# - 
v0	#x[1]
v2	#x[3]
n2
o5	#^
o1	# - 
v3	#y[1]
v5	#y[3]
n2
n8
o3	#/
n1
o5	#^
o0	# + 
o5	#^
o1	# - 
v2	#x[3]
v1	#x[2]
n2
o5	#^
o1	# - 
v5	#y[3]
v4	#y[2]
n2
n8
x6	# initial guess
0 -1
1 0
2 1
3 0
4 -1
5 1
r	#12 ranges (rhs's)
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
b	#6 bounds (on variables)
0 -1.e8 1
0 -1.e8 1
0 -1.e8 1
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k5	#intermediate Jacobian column lengths
4
8
12
16
20
J0 2
0 0.30901699437494745
3 0.9510565162951535
J1 2
0 -0.8090169943749473
3 0.5877852522924732
J2 2
0 -0.8090169943749476
3 -0.587785252292473
J3 2
0 0.30901699437494723
3 -0.9510565162951536
J4 2
1 0.30901699437494745
4 0.9510565162951535
J5 2
1 -0.8090169943749473
4 0.5877852522924732
J6 2
1 -0.8090169943749476
4 -0.587785252292473
J7 2
1 0.30901699437494723
4 -0.9510565162951536
J8 2
2 0.30901699437494745
5 0.9510565162951535
J9 2
2 -0.8090169943749473
5 0.5877852522924732
J10 2
2 -0.8090169943749476
5 -0.587785252292473
J11 2
2 0.30901699437494723
5 -0.9510565162951536
G0 6
0 0
1 0
2 0
3 0
4 0
5 0
