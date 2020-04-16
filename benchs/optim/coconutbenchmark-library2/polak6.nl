g9 0 1 0 4 20130207 0 4 0 240	# problem polak6
 5 4 1 0 0	# vars, constraints, objectives, ranges, eqns
 4 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 4 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 20 1	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
o54	#sumlist
6
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n2
o5	#^
o1	# - 
v1	#x[2]
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
n2
o2	#*
n2
o5	#^
v2	#x[3]
n2
o5	#^
v3	#x[4]
n2
o2	#*
n5
o5	#^
o0	# + 
n1
v3	#x[4]
n4
o2	#*
n5
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
C1	#cons2
o54	#sumlist
6
o2	#*
n11
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n2
o2	#*
n11
o5	#^
o1	# - 
v1	#x[2]
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
n2
o2	#*
n12
o5	#^
v2	#x[3]
n2
o2	#*
n11
o5	#^
v3	#x[4]
n2
o2	#*
n-5
o5	#^
o0	# + 
n1
v3	#x[4]
n4
o2	#*
n15
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
C2	#cons3
o54	#sumlist
6
o2	#*
n11
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n2
o2	#*
n21
o5	#^
o1	# - 
v1	#x[2]
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
n2
o2	#*
n12
o5	#^
v2	#x[3]
n2
o2	#*
n21
o5	#^
v3	#x[4]
n2
o2	#*
n15
o5	#^
o0	# + 
n1
v3	#x[4]
n4
o2	#*
n5
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
C3	#cons4
o54	#sumlist
6
o2	#*
n11
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n2
o2	#*
n11
o5	#^
o1	# - 
v1	#x[2]
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
n2
o2	#*
n12
o5	#^
v2	#x[3]
n2
o5	#^
v3	#x[4]
n2
o2	#*
n-15
o5	#^
o0	# + 
n1
v3	#x[4]
n4
o2	#*
n15
o5	#^
o1	# - 
v0	#x[1]
o5	#^
o0	# + 
n1
v3	#x[4]
n4
n4
O0 0	#f
n0
x4	# initial guess
0 0
1 0
2 0
3 0
r	#4 ranges (rhs's)
1 0
1 80
1 100
1 50
b	#5 bounds (on variables)
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k4	#intermediate Jacobian column lengths
4
8
12
16
J0 5
0 -5
1 -5
2 -21
3 7
4 -1
J1 5
0 5
1 -15
2 -11
3 -3
4 -1
J2 5
0 -15
1 -5
2 -21
3 -3
4 -1
J3 5
0 15
1 -15
2 -21
3 -3
4 -1
G0 1
4 1
