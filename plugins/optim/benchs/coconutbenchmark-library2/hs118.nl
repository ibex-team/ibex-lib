g3 6 1 0	# problem hs118
 15 17 1 12 0	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 15 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 39 15	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#constr1[1]
n0
C1	#constr1[2]
n0
C2	#constr1[3]
n0
C3	#constr1[4]
n0
C4	#constr2[1]
n0
C5	#constr2[2]
n0
C6	#constr2[3]
n0
C7	#constr2[4]
n0
C8	#constr3[1]
n0
C9	#constr3[2]
n0
C10	#constr3[3]
n0
C11	#constr3[4]
n0
C12	#constr4
n0
C13	#constr5
n0
C14	#constr6
n0
C15	#constr7
n0
C16	#constr8
n0
O0 0	#obj
o54	#sumlist
15
o2	#*
n0.0001
o5	#^
v0	#x[1]
n2
o2	#*
n0.0001
o5	#^
v1	#x[2]
n2
o2	#*
n0.00015
o5	#^
v2	#x[3]
n2
o2	#*
n0.0001
o5	#^
v3	#x[4]
n2
o2	#*
n0.0001
o5	#^
v4	#x[5]
n2
o2	#*
n0.00015
o5	#^
v5	#x[6]
n2
o2	#*
n0.0001
o5	#^
v6	#x[7]
n2
o2	#*
n0.0001
o5	#^
v7	#x[8]
n2
o2	#*
n0.00015
o5	#^
v8	#x[9]
n2
o2	#*
n0.0001
o5	#^
v9	#x[10]
n2
o2	#*
n0.0001
o5	#^
v10	#x[11]
n2
o2	#*
n0.00015
o5	#^
v11	#x[12]
n2
o2	#*
n0.0001
o5	#^
v12	#x[13]
n2
o2	#*
n0.0001
o5	#^
v13	#x[14]
n2
o2	#*
n0.00015
o5	#^
v14	#x[15]
n2
x15	# initial guess
0 20
1 55
2 15
3 20
4 60
5 20
6 20
7 60
8 20
9 20
10 60
11 20
12 20
13 60
14 20
r	#17 ranges (rhs's)
0 -7 6
0 -7 6
0 -7 6
0 -7 6
0 -7 7
0 -7 7
0 -7 7
0 -7 7
0 -7 6
0 -7 6
0 -7 6
0 -7 6
2 60
2 50
2 70
2 85
2 100
b	#15 bounds (on variables)
0 8 21
0 43 57
0 3 16
0 0 90
0 0 120
0 0 60
0 0 90
0 0 120
0 0 60
0 0 90
0 0 120
0 0 60
0 0 90
0 0 120
0 0 60
k14	#intermediate Jacobian column lengths
2
4
6
9
12
15
18
21
24
27
30
33
35
37
J0 2
0 -1
3 1
J1 2
3 -1
6 1
J2 2
6 -1
9 1
J3 2
9 -1
12 1
J4 2
1 -1
4 1
J5 2
4 -1
7 1
J6 2
7 -1
10 1
J7 2
10 -1
13 1
J8 2
2 -1
5 1
J9 2
5 -1
8 1
J10 2
8 -1
11 1
J11 2
11 -1
14 1
J12 3
0 1
1 1
2 1
J13 3
3 1
4 1
5 1
J14 3
6 1
7 1
8 1
J15 3
9 1
10 1
11 1
J16 3
12 1
13 1
14 1
G0 15
0 2.3
1 1.7
2 2.2
3 2.3
4 1.7
5 2.2
6 2.3
7 1.7
8 2.2
9 2.3
10 1.7
11 2.2
12 2.3
13 1.7
14 2.2
