g3 3 1 0	# problem ex2_1_7
 20 10 1 0 0	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 20 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 164 20	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#e2
n0
C1	#e3
n0
C2	#e4
n0
C3	#e5
n0
C4	#e6
n0
C5	#e7
n0
C6	#e8
n0
C7	#e9
n0
C8	#e10
n0
C9	#e11
n0
O0 0	#obj
o54	#sumlist
20
o2	#*
n-0.5
o5	#^
o0	# + 
n-2
v0	#x1
n2
o16	#-
o5	#^
o0	# + 
n-2
v1	#x2
n2
o2	#*
n-1.5
o5	#^
o0	# + 
n-2
v2	#x3
n2
o2	#*
n-2
o5	#^
o0	# + 
n-2
v3	#x4
n2
o2	#*
n-2.5
o5	#^
o0	# + 
n-2
v4	#x5
n2
o2	#*
n-3
o5	#^
o0	# + 
n-2
v5	#x6
n2
o2	#*
n-3.5
o5	#^
o0	# + 
n-2
v6	#x7
n2
o2	#*
n-4
o5	#^
o0	# + 
n-2
v7	#x8
n2
o2	#*
n-4.5
o5	#^
o0	# + 
n-2
v8	#x9
n2
o2	#*
n-5
o5	#^
o0	# + 
n-2
v9	#x10
n2
o2	#*
n-5.5
o5	#^
o0	# + 
n-2
v10	#x11
n2
o2	#*
n-6
o5	#^
o0	# + 
n-2
v11	#x12
n2
o2	#*
n-6.5
o5	#^
o0	# + 
n-2
v12	#x13
n2
o2	#*
n-7
o5	#^
o0	# + 
n-2
v13	#x14
n2
o2	#*
n-7.5
o5	#^
o0	# + 
n-2
v14	#x15
n2
o2	#*
n-8
o5	#^
o0	# + 
n-2
v15	#x16
n2
o2	#*
n-8.5
o5	#^
o0	# + 
n-2
v16	#x17
n2
o2	#*
n-9
o5	#^
o0	# + 
n-2
v17	#x18
n2
o2	#*
n-9.5
o5	#^
o0	# + 
n-2
v18	#x19
n2
o2	#*
n-10
o5	#^
o0	# + 
n-2
v19	#x20
n2
x6	# initial guess
2 1.04289
10 1.74674
12 0.43147
15 4.43305
17 15.85893
19 16.4889
r	#10 ranges (rhs's)
1 -5
1 2
1 -1
1 -3
1 5
1 4
1 -1
1 0
1 9
1 40
b	#20 bounds (on variables)
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
2 0
k19	#intermediate Jacobian column lengths
8
16
24
33
42
50
57
65
73
81
89
97
105
113
121
130
139
148
156
J0 16
0 -3
1 7
3 -5
4 1
5 1
7 2
8 -1
9 -1
10 -9
11 3
12 5
15 1
16 7
17 -7
18 -4
19 -6
J1 16
0 7
2 -5
3 1
4 1
6 2
7 -1
8 -1
9 -9
10 3
11 5
14 1
15 7
16 -7
17 -4
18 -6
19 -3
J2 16
1 -5
2 1
3 1
5 2
6 -1
7 -1
8 -9
9 3
10 5
13 1
14 7
15 -7
16 -4
17 -6
18 -3
19 7
J3 16
0 -5
1 1
2 1
4 2
5 -1
6 -1
7 -9
8 3
9 5
12 1
13 7
14 -7
15 -4
16 -6
17 -3
18 7
J4 16
0 1
1 1
3 2
4 -1
5 -1
6 -9
7 3
8 5
11 1
12 7
13 -7
14 -4
15 -6
16 -3
17 7
19 -5
J5 16
0 1
2 2
3 -1
4 -1
5 -9
6 3
7 5
10 1
11 7
12 -7
13 -4
14 -6
15 -3
16 7
18 -5
19 1
J6 16
1 2
2 -1
3 -1
4 -9
5 3
6 5
9 1
10 7
11 -7
12 -4
13 -6
14 -3
15 7
17 -5
18 1
19 1
J7 16
0 2
1 -1
2 -1
3 -9
4 3
5 5
8 1
9 7
10 -7
11 -4
12 -6
13 -3
14 7
16 -5
17 1
18 1
J8 16
0 -1
1 -1
2 -9
3 3
4 5
7 1
8 7
9 -7
10 -4
11 -6
12 -3
13 7
15 -5
16 1
17 1
19 2
J9 20
0 1
1 1
2 1
3 1
4 1
5 1
6 1
7 1
8 1
9 1
10 1
11 1
12 1
13 1
14 1
15 1
16 1
17 1
18 1
19 1
G0 20
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
9 0
10 0
11 0
12 0
13 0
14 0
15 0
16 0
17 0
18 0
19 0
