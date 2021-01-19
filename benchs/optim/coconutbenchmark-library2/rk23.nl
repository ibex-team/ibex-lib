g3 0 1 0	# problem rk23
 17 11 1 0 11	# vars, constraints, objectives, ranges, eqns
 7 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 7 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 43 6	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons5
o0	# + 
o2	#*
v3	#B2
v0	#C2
o2	#*
v4	#B3
v1	#C3
C1	#cons6
o0	# + 
o2	#*
v5	#BB2
v0	#C2
o2	#*
v6	#BB3
v1	#C3
C2	#cons7
o0	# + 
o2	#*
v5	#BB2
o5	#^
v0	#C2
n2
o2	#*
v6	#BB3
o5	#^
v1	#C3
n2
C3	#cons8
o2	#*
o2	#*
v6	#BB3
v2	#A32
v0	#C2
C4	#cons9
o0	# + 
o2	#*
o2	#*
n4
v5	#BB2
o5	#^
v0	#C2
n3
o2	#*
o2	#*
n4
v6	#BB3
o5	#^
v1	#C3
n3
C5	#cons10
o2	#*
o2	#*
o2	#*
o2	#*
n8
v6	#BB3
v1	#C3
v2	#A32
v0	#C2
C6	#cons11
o2	#*
o2	#*
o2	#*
n12
v6	#BB3
v2	#A32
o5	#^
v0	#C2
n2
C7	#cons1
n0
C8	#cons2
n0
C9	#cons3
n0
C10	#cons4
n0
O0 0	#f
n0
x11	# initial guess
0 1
1 0.5
2 0.25
3 0.5
4 0
5 0.16666666666666666
6 0.6666666666666666
7 1
8 0.25
9 0.5
10 0.16666666666666666
r	#11 ranges (rhs's)
4 0.5
4 0.5
4 0.3333333333333333
4 0.16666666666666666
4 1
4 1
4 1
4 0
4 0
4 1
4 1
b	#17 bounds (on variables)
3
3
3
3
3
3
3
3
3
3
3
0 0 1e+08
0 0 1e+08
0 0 1e+08
0 0 1e+08
0 0 1e+08
0 0 1e+08
k16	#intermediate Jacobian column lengths
8
14
18
20
22
26
33
34
35
36
37
38
38
39
41
42
J0 4
0 0
1 0
3 0
4 0
J1 4
0 0
1 0
5 0
6 0
J2 4
0 0
1 0
5 0
6 0
J3 3
0 0
2 0
6 0
J4 6
0 0
1 0
5 0
6 0
11 1
14 -1
J5 6
0 0
1 0
2 0
6 0
13 1
14 -1
J6 5
0 0
2 0
6 0
15 1
16 -1
J7 2
0 -1
7 1
J8 3
1 -1
2 1
8 1
J9 3
3 1
4 1
9 1
J10 3
5 1
6 1
10 1
G0 6
11 1
12 1
13 1
14 1
15 1
16 1
