g3 10 1 0	# problem optcntrl
 28 20 1 0 19	# vars, constraints, objectives, ranges, eqns
 10 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 18 9 9	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 71 9	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#obj_bnd
o16	#-
o54	#sumlist
9
o2	#*
o2	#*
n0.5
v0	#x2
v0	#x2
o2	#*
o2	#*
n0.5
v1	#x3
v1	#x3
o2	#*
o2	#*
n0.5
v2	#x4
v2	#x4
o2	#*
o2	#*
n0.5
v3	#x5
v3	#x5
o2	#*
o2	#*
n0.5
v4	#x6
v4	#x6
o2	#*
o2	#*
n0.5
v5	#x7
v5	#x7
o2	#*
o2	#*
n0.5
v6	#x8
v6	#x8
o2	#*
o2	#*
n0.5
v7	#x9
v7	#x9
o2	#*
o2	#*
n0.5
v8	#x10
v8	#x10
C1	#c1
o2	#*
o2	#*
n0.01
v9	#y1
v9	#y1
C2	#c2
o2	#*
o2	#*
n0.01
v10	#y2
v10	#y2
C3	#c3
o2	#*
o2	#*
n0.01
v11	#y3
v11	#y3
C4	#c4
o2	#*
o2	#*
n0.01
v12	#y4
v12	#y4
C5	#c5
o2	#*
o2	#*
n0.01
v13	#y5
v13	#y5
C6	#c6
o2	#*
o2	#*
n0.01
v14	#y6
v14	#y6
C7	#c7
o2	#*
o2	#*
n0.01
v15	#y7
v15	#y7
C8	#c8
o2	#*
o2	#*
n0.01
v16	#y8
v16	#y8
C9	#c9
o2	#*
o2	#*
n0.01
v17	#y9
v17	#y9
C10	#c0
n0
C11	#b1
n0
C12	#b2
n0
C13	#b3
n0
C14	#b4
n0
C15	#b5
n0
C16	#b6
n0
C17	#b7
n0
C18	#b8
n0
C19	#b9
n0
O0 0	#obj
o54	#sumlist
10
o2	#*
o2	#*
n0.5
v0	#x2
v0	#x2
o2	#*
o2	#*
n0.5
v1	#x3
v1	#x3
o2	#*
o2	#*
n0.5
v2	#x4
v2	#x4
o2	#*
o2	#*
n0.5
v3	#x5
v3	#x5
o2	#*
o2	#*
n0.5
v4	#x6
v4	#x6
o2	#*
o2	#*
n0.5
v5	#x7
v5	#x7
o2	#*
o2	#*
n0.5
v6	#x8
v6	#x8
o2	#*
o2	#*
n0.5
v7	#x9
v7	#x9
o2	#*
o2	#*
n0.5
v8	#x10
v8	#x10
n100
r	#20 ranges (rhs's)
1 100
4 -0.04
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 -0.04
4 10
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
b	#28 bounds (on variables)
3
3
3
3
3
3
3
3
2 0
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
2 -1
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
0 -0.2 0.2
k27	#intermediate Jacobian column lengths
4
8
12
16
20
24
28
32
34
37
40
43
46
49
52
55
58
61
62
63
64
65
66
67
68
69
70
J0 9
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
J1 3
9 -1
10 1
19 -0.2
J2 4
0 0.004
10 -1
11 1
20 -0.2
J3 4
1 0.004
11 -1
12 1
21 -0.2
J4 4
2 0.004
12 -1
13 1
22 -0.2
J5 4
3 0.004
13 -1
14 1
23 -0.2
J6 4
4 0.004
14 -1
15 1
24 -0.2
J7 4
5 0.004
15 -1
16 1
25 -0.2
J8 4
6 0.004
16 -1
17 1
26 -0.2
J9 3
7 0.004
17 -1
27 -0.2
J10 2
9 1
18 -0.2
J11 2
0 1
9 -0.2
J12 3
0 -1
1 1
10 -0.2
J13 3
1 -1
2 1
11 -0.2
J14 3
2 -1
3 1
12 -0.2
J15 3
3 -1
4 1
13 -0.2
J16 3
4 -1
5 1
14 -0.2
J17 3
5 -1
6 1
15 -0.2
J18 3
6 -1
7 1
16 -0.2
J19 3
7 -1
8 1
17 -0.2
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
