g3 2 1 0	# problem hs109
 9 10 1 0 6	# vars, constraints, objectives, ranges, eqns
 8 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 9 2 2	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 42 2	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#C3
o1	# - 
o16	#-
o5	#^
v7	#x[8]
n2
o5	#^
v0	#x[1]
n2
C1	#C4
o1	# - 
o16	#-
o5	#^
v8	#x[9]
n2
o5	#^
v1	#x[2]
n2
C2	#C5
o54	#sumlist
3
o2	#*
o2	#*
v4	#x[5]
v5	#x[6]
o41	#sin
o0	# + 
n-0.25
o16	#-
v2	#x[3]
o2	#*
o2	#*
v4	#x[5]
v6	#x[7]
o41	#sin
o0	# + 
n-0.25
o16	#-
v3	#x[4]
o2	#*
n0.4948079185090459
o5	#^
v4	#x[5]
n2
C3	#C6
o54	#sumlist
3
o2	#*
o2	#*
v4	#x[5]
v5	#x[6]
o41	#sin
o0	# + 
n-0.25
v2	#x[3]
o2	#*
o2	#*
v5	#x[6]
v6	#x[7]
o41	#sin
o0	# + 
n-0.25
o1	# - 
v2	#x[3]
v3	#x[4]
o2	#*
n0.4948079185090459
o5	#^
v5	#x[6]
n2
C4	#C7
o54	#sumlist
3
o2	#*
o2	#*
v4	#x[5]
v6	#x[7]
o41	#sin
o0	# + 
n-0.25
v3	#x[4]
o2	#*
o2	#*
v5	#x[6]
v6	#x[7]
o41	#sin
o0	# + 
n-0.25
o1	# - 
v3	#x[4]
v2	#x[3]
o2	#*
n0.4948079185090459
o5	#^
v6	#x[7]
n2
C5	#C8
o54	#sumlist
4
o2	#*
o2	#*
v4	#x[5]
v5	#x[6]
o46	#cos
o0	# + 
n-0.25
o16	#-
v2	#x[3]
o2	#*
o2	#*
v4	#x[5]
v6	#x[7]
o46	#cos
o0	# + 
n-0.25
o16	#-
v3	#x[4]
o2	#*
n-1.9378248434212895
o5	#^
v4	#x[5]
n2
o2	#*
n0.0377975808
o5	#^
v4	#x[5]
n2
C6	#C9
o54	#sumlist
4
o2	#*
o2	#*
v4	#x[5]
v5	#x[6]
o46	#cos
o0	# + 
n-0.25
v2	#x[3]
o2	#*
o2	#*
v5	#x[6]
v6	#x[7]
o46	#cos
o0	# + 
n-0.25
o1	# - 
v2	#x[3]
v3	#x[4]
o2	#*
n-1.9378248434212895
o5	#^
v5	#x[6]
n2
o2	#*
n0.0377975808
o5	#^
v5	#x[6]
n2
C7	#C10
o54	#sumlist
4
o2	#*
o2	#*
v4	#x[5]
v6	#x[7]
o46	#cos
o0	# + 
n-0.25
v3	#x[4]
o2	#*
o2	#*
v5	#x[6]
v6	#x[7]
o46	#cos
o0	# + 
n-0.25
o1	# - 
v3	#x[4]
v2	#x[3]
o2	#*
n-1.9378248434212895
o5	#^
v6	#x[7]
n2
o2	#*
n0.0377975808
o5	#^
v6	#x[7]
n2
C8	#C1
n0
C9	#C2
n0
O0 0	#obj
o0	# + 
o2	#*
n1e-06
o5	#^
v0	#x[1]
n3
o2	#*
n5.22074e-07
o5	#^
v1	#x[2]
n3
x9	# initial guess
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
r	#10 ranges (rhs's)
2 -2250000
2 -2250000
4 -20070.4
4 -20070.4
4 -44244.143104
4 10035.2
4 10035.2
4 -1150.937088
2 -0.55
2 -0.55
b	#9 bounds (on variables)
2 0
2 0
0 -0.55 0.55
0 -0.55 0.55
0 196 252
0 196 252
0 196 252
0 -400 800
0 -400 800
k8	#intermediate Jacobian column lengths
2
4
12
20
26
32
38
40
J0 2
0 0
7 0
J1 2
1 0
8 0
J2 6
0 -50.176
2 0
3 0
4 0
5 0
6 0
J3 6
1 -50.176
2 0
3 0
4 0
5 0
6 0
J4 5
2 0
3 0
4 0
5 0
6 0
J5 6
2 0
3 0
4 0
5 0
6 0
7 50.176
J6 6
2 0
3 0
4 0
5 0
6 0
8 50.176
J7 5
2 0
3 0
4 0
5 0
6 0
J8 2
2 -1
3 1
J9 2
2 1
3 -1
G0 2
0 3
1 2
