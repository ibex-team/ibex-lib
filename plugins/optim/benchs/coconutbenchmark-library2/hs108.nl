g3 1 1 0	# problem hs108
 9 13 1 0 0	# vars, constraints, objectives, ranges, eqns
 13 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 9 9 9	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 39 9	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#c1
o1	# - 
o16	#-
o5	#^
v3	#x[4]
n2
o5	#^
v2	#x[3]
n2
C1	#c2
o1	# - 
o16	#-
o5	#^
v5	#x[6]
n2
o5	#^
v4	#x[5]
n2
C2	#c3
o16	#-
o5	#^
v8	#x[9]
n2
C3	#c4
o1	# - 
o16	#-
o5	#^
o1	# - 
v1	#x[2]
v8	#x[9]
n2
o5	#^
v0	#x[1]
n2
C4	#c5
o1	# - 
o16	#-
o5	#^
o1	# - 
v1	#x[2]
v5	#x[6]
n2
o5	#^
o1	# - 
v0	#x[1]
v4	#x[5]
n2
C5	#c6
o1	# - 
o16	#-
o5	#^
o1	# - 
v1	#x[2]
v7	#x[8]
n2
o5	#^
o1	# - 
v0	#x[1]
v6	#x[7]
n2
C6	#c7
o1	# - 
o16	#-
o5	#^
o1	# - 
v3	#x[4]
v7	#x[8]
n2
o5	#^
o1	# - 
v2	#x[3]
v6	#x[7]
n2
C7	#c8
o1	# - 
o16	#-
o5	#^
o1	# - 
v3	#x[4]
v5	#x[6]
n2
o5	#^
o1	# - 
v2	#x[3]
v4	#x[5]
n2
C8	#c9
o1	# - 
o16	#-
o5	#^
o1	# - 
v7	#x[8]
v8	#x[9]
n2
o5	#^
v6	#x[7]
n2
C9	#c10
o1	# - 
o2	#*
v0	#x[1]
v3	#x[4]
o2	#*
v1	#x[2]
v2	#x[3]
C10	#c11
o2	#*
v2	#x[3]
v8	#x[9]
C11	#c12
o16	#-
o2	#*
v4	#x[5]
v8	#x[9]
C12	#c13
o1	# - 
o2	#*
v4	#x[5]
v7	#x[8]
o2	#*
v5	#x[6]
v6	#x[7]
O0 0	#obj
o54	#sumlist
6
o2	#*
n-0.5
o2	#*
v0	#x[1]
v3	#x[4]
o2	#*
n0.5
o2	#*
v1	#x[2]
v2	#x[3]
o2	#*
n-0.5
o2	#*
v2	#x[3]
v8	#x[9]
o2	#*
n0.5
o2	#*
v4	#x[5]
v8	#x[9]
o2	#*
n-0.5
o2	#*
v4	#x[5]
v7	#x[8]
o2	#*
n0.5
o2	#*
v5	#x[6]
v6	#x[7]
x9	# initial guess
0 1
1 1
2 1
3 1
4 1
5 1
6 1
7 1
8 1
r	#13 ranges (rhs's)
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 0
2 0
2 0
2 0
b	#9 bounds (on variables)
3
3
3
3
3
3
3
3
2 0
k8	#intermediate Jacobian column lengths
4
8
13
17
22
26
30
34
J0 2
2 0
3 0
J1 2
4 0
5 0
J2 1
8 0
J3 3
0 0
1 0
8 0
J4 4
0 0
1 0
4 0
5 0
J5 4
0 0
1 0
6 0
7 0
J6 4
2 0
3 0
6 0
7 0
J7 4
2 0
3 0
4 0
5 0
J8 3
6 0
7 0
8 0
J9 4
0 0
1 0
2 0
3 0
J10 2
2 0
8 0
J11 2
4 0
8 0
J12 4
4 0
5 0
6 0
7 0
G0 9
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
