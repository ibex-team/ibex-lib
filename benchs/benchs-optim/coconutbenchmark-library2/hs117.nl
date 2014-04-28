g3 0 1 0	# problem hs117
 15 5 1 0 0	# vars, constraints, objectives, ranges, eqns
 5 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 5 5 5	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 62 15	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#constr1[1]
o2	#*
n12
o5	#^
v0	#x[11]
n2
C1	#constr1[2]
o2	#*
n24
o5	#^
v1	#x[12]
n2
C2	#constr1[3]
o2	#*
n30
o5	#^
v2	#x[13]
n2
C3	#constr1[4]
o2	#*
n18
o5	#^
v3	#x[14]
n2
C4	#constr1[5]
o2	#*
n6
o5	#^
v4	#x[15]
n2
O0 0	#obj
o54	#sumlist
30
o2	#*
o2	#*
n30
v0	#x[11]
v0	#x[11]
o2	#*
o2	#*
n-20
v1	#x[12]
v0	#x[11]
o2	#*
o2	#*
n-10
v2	#x[13]
v0	#x[11]
o2	#*
o2	#*
n32
v3	#x[14]
v0	#x[11]
o2	#*
o2	#*
n-10
v4	#x[15]
v0	#x[11]
o2	#*
o2	#*
n-20
v0	#x[11]
v1	#x[12]
o2	#*
o2	#*
n39
v1	#x[12]
v1	#x[12]
o2	#*
o2	#*
n-6
v2	#x[13]
v1	#x[12]
o2	#*
o2	#*
n-31
v3	#x[14]
v1	#x[12]
o2	#*
o2	#*
n32
v4	#x[15]
v1	#x[12]
o2	#*
o2	#*
n-10
v0	#x[11]
v2	#x[13]
o2	#*
o2	#*
n-6
v1	#x[12]
v2	#x[13]
o2	#*
o2	#*
n10
v2	#x[13]
v2	#x[13]
o2	#*
o2	#*
n-6
v3	#x[14]
v2	#x[13]
o2	#*
o2	#*
n-10
v4	#x[15]
v2	#x[13]
o2	#*
o2	#*
n32
v0	#x[11]
v3	#x[14]
o2	#*
o2	#*
n-31
v1	#x[12]
v3	#x[14]
o2	#*
o2	#*
n-6
v2	#x[13]
v3	#x[14]
o2	#*
o2	#*
n39
v3	#x[14]
v3	#x[14]
o2	#*
o2	#*
n-20
v4	#x[15]
v3	#x[14]
o2	#*
o2	#*
n-10
v0	#x[11]
v4	#x[15]
o2	#*
o2	#*
n32
v1	#x[12]
v4	#x[15]
o2	#*
o2	#*
n-10
v2	#x[13]
v4	#x[15]
o2	#*
o2	#*
n-20
v3	#x[14]
v4	#x[15]
o2	#*
o2	#*
n30
v4	#x[15]
v4	#x[15]
o2	#*
n8
o5	#^
v0	#x[11]
n3
o2	#*
n16
o5	#^
v1	#x[12]
n3
o2	#*
n20
o5	#^
v2	#x[13]
n3
o2	#*
n12
o5	#^
v3	#x[14]
n3
o2	#*
n4
o5	#^
v4	#x[15]
n3
x15	# initial guess
0 0.001
1 0.001
2 0.001
3 0.001
4 0.001
5 0.001
6 0.001
7 0.001
8 0.001
9 0.001
10 0.001
11 60
12 0.001
13 0.001
14 0.001
r	#5 ranges (rhs's)
2 15
2 27
2 36
2 18
2 12
b	#15 bounds (on variables)
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
0 0 1.e8
k14	#intermediate Jacobian column lengths
5
10
15
20
25
28
31
33
36
40
42
47
52
57
J0 12
0 60
1 -40
2 -20
3 64
4 -20
5 16
7 3.5
10 -2
11 1
12 1
13 -1
14 -1
J1 13
0 -40
1 78
2 -12
3 -62
4 64
5 -2
6 2
8 2
9 9
11 1
12 2
13 -2
14 -1
J2 12
0 -20
1 -12
2 20
3 -12
4 -20
7 -2
9 2
10 4
11 1
12 3
13 -3
14 -1
J3 13
0 64
1 -62
2 -12
3 78
4 -40
5 -1
6 -4
8 4
9 -1
11 1
12 2
13 -4
14 -1
J4 12
0 -20
1 64
2 -20
3 -40
4 60
6 -2
8 1
9 2.8
11 1
12 1
13 -5
14 -1
G0 15
0 0
1 0
2 0
3 0
4 0
5 40
6 2
7 0.25
8 4
9 4
10 1
11 40
12 60
13 -5
14 -1
