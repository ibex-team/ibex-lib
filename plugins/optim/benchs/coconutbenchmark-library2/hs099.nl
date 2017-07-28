g3 1 1 0	# problem hs099
 19 14 1 0 14	# vars, constraints, objectives, ranges, eqns
 14 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 7 7 7	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 44 7	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#constr5[2]
o2	#*
n-15625
o41	#sin
v0	#x[1]
C1	#constr5[3]
o2	#*
n-15625
o41	#sin
v1	#x[2]
C2	#constr5[4]
o2	#*
n-93750
o41	#sin
v2	#x[3]
C3	#constr5[5]
o2	#*
n-93750
o41	#sin
v3	#x[4]
C4	#constr5[6]
o2	#*
n-93750
o41	#sin
v4	#x[5]
C5	#constr5[7]
o2	#*
n-405000
o41	#sin
v5	#x[6]
C6	#constr5[8]
o2	#*
n-405000
o41	#sin
v6	#x[7]
C7	#constr6[2]
o2	#*
n-1250
o41	#sin
v0	#x[1]
C8	#constr6[3]
o2	#*
n-1250
o41	#sin
v1	#x[2]
C9	#constr6[4]
o2	#*
n-3750
o41	#sin
v2	#x[3]
C10	#constr6[5]
o2	#*
n-3750
o41	#sin
v3	#x[4]
C11	#constr6[6]
o2	#*
n-3750
o41	#sin
v4	#x[5]
C12	#constr6[7]
o2	#*
n-9000
o41	#sin
v5	#x[6]
C13	#constr6[8]
o2	#*
n-9000
o41	#sin
v6	#x[7]
O0 0	#obj
o16	#-
o5	#^
o54	#sumlist
7
o2	#*
n1250
o46	#cos
v0	#x[1]
o2	#*
n1250
o46	#cos
v1	#x[2]
o2	#*
n3750
o46	#cos
v2	#x[3]
o2	#*
n3750
o46	#cos
v3	#x[4]
o2	#*
n3750
o46	#cos
v4	#x[5]
o2	#*
n9000
o46	#cos
v5	#x[6]
o2	#*
n9000
o46	#cos
v6	#x[7]
n2
x7	# initial guess
0 0.5
1 0.5
2 0.5
3 0.5
4 0.5
5 0.5
6 0.5
r	#14 ranges (rhs's)
4 -10000
4 -10000
4 -40000
4 -40000
4 -40000
4 -129600
4 -229600
4 -800
4 -800
4 -1600
4 -1600
4 -1600
4 -2880
4 -3880
b	#19 bounds (on variables)
0 0 1.58
0 0 1.58
0 0 1.58
0 0 1.58
0 0 1.58
0 0 1.58
0 0 1.58
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k18	#intermediate Jacobian column lengths
2
4
6
8
10
12
14
16
18
20
22
24
26
29
32
35
38
41
J0 2
0 0
7 1
J1 4
1 0
7 -1
8 1
13 -25
J2 4
2 0
8 -1
9 1
14 -50
J3 4
3 0
9 -1
10 1
15 -50
J4 4
4 0
10 -1
11 1
16 -50
J5 4
5 0
11 -1
12 1
17 -90
J6 3
6 0
12 -1
18 -90
J7 2
0 0
13 1
J8 3
1 0
13 -1
14 1
J9 3
2 0
14 -1
15 1
J10 3
3 0
15 -1
16 1
J11 3
4 0
16 -1
17 1
J12 3
5 0
17 -1
18 1
J13 2
6 0
18 -1
G0 7
0 0
1 0
2 0
3 0
4 0
5 0
6 0
