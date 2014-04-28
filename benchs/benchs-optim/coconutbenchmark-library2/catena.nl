g3 0 1 0	# problem catena
 32 11 1 0 11	# vars, constraints, objectives, ranges, eqns
 11 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 32 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 62 11	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1[1]
o54	#sumlist
3
o5	#^
v0	#x[1]
n2
o5	#^
v10	#y[1]
n2
o5	#^
v21	#z[1]
n2
C1	#cons1[2]
o54	#sumlist
3
o5	#^
o1	# - 
v1	#x[2]
v0	#x[1]
n2
o5	#^
o1	# - 
v11	#y[2]
v10	#y[1]
n2
o5	#^
o1	# - 
v22	#z[2]
v21	#z[1]
n2
C2	#cons1[3]
o54	#sumlist
3
o5	#^
o1	# - 
v2	#x[3]
v1	#x[2]
n2
o5	#^
o1	# - 
v12	#y[3]
v11	#y[2]
n2
o5	#^
o1	# - 
v23	#z[3]
v22	#z[2]
n2
C3	#cons1[4]
o54	#sumlist
3
o5	#^
o1	# - 
v3	#x[4]
v2	#x[3]
n2
o5	#^
o1	# - 
v13	#y[4]
v12	#y[3]
n2
o5	#^
o1	# - 
v24	#z[4]
v23	#z[3]
n2
C4	#cons1[5]
o54	#sumlist
3
o5	#^
o1	# - 
v4	#x[5]
v3	#x[4]
n2
o5	#^
o1	# - 
v14	#y[5]
v13	#y[4]
n2
o5	#^
o1	# - 
v25	#z[5]
v24	#z[4]
n2
C5	#cons1[6]
o54	#sumlist
3
o5	#^
o1	# - 
v5	#x[6]
v4	#x[5]
n2
o5	#^
o1	# - 
v15	#y[6]
v14	#y[5]
n2
o5	#^
o1	# - 
v26	#z[6]
v25	#z[5]
n2
C6	#cons1[7]
o54	#sumlist
3
o5	#^
o1	# - 
v6	#x[7]
v5	#x[6]
n2
o5	#^
o1	# - 
v16	#y[7]
v15	#y[6]
n2
o5	#^
o1	# - 
v27	#z[7]
v26	#z[6]
n2
C7	#cons1[8]
o54	#sumlist
3
o5	#^
o1	# - 
v7	#x[8]
v6	#x[7]
n2
o5	#^
o1	# - 
v17	#y[8]
v16	#y[7]
n2
o5	#^
o1	# - 
v28	#z[8]
v27	#z[7]
n2
C8	#cons1[9]
o54	#sumlist
3
o5	#^
o1	# - 
v8	#x[9]
v7	#x[8]
n2
o5	#^
o1	# - 
v18	#y[9]
v17	#y[8]
n2
o5	#^
o1	# - 
v29	#z[9]
v28	#z[8]
n2
C9	#cons1[10]
o54	#sumlist
3
o5	#^
o1	# - 
v9	#x[10]
v8	#x[9]
n2
o5	#^
o1	# - 
v19	#y[10]
v18	#y[9]
n2
o5	#^
o1	# - 
v30	#z[10]
v29	#z[9]
n2
C10	#cons1[11]
o54	#sumlist
3
o5	#^
o1	# - 
n6.6
v9	#x[10]
n2
o5	#^
o1	# - 
v20	#y[11]
v19	#y[10]
n2
o5	#^
o1	# - 
v31	#z[11]
v30	#z[10]
n2
O0 0	#f
n0
x32	# initial guess
0 0.6
1 1.2
2 1.7999999999999998
3 2.4
4 3
5 3.5999999999999996
6 4.199999999999999
7 4.8
8 5.3999999999999995
9 6
10 -0.6
11 -1.2
12 -1.7999999999999998
13 -2.4
14 -3
15 -3.5999999999999996
16 -4.199999999999999
17 -4.8
18 -5.3999999999999995
19 -6
20 -6.6
21 0
22 0
23 0
24 0
25 0
26 0
27 0
28 0
29 0
30 0
31 0
r	#11 ranges (rhs's)
4 1
4 1
4 1
4 1
4 1
4 1
4 1
4 1
4 1
4 1
4 1
b	#32 bounds (on variables)
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
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k31	#intermediate Jacobian column lengths
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
28
30
32
34
36
38
40
41
43
45
47
49
51
53
55
57
59
61
J0 3
0 0
10 0
21 0
J1 6
0 0
1 0
10 0
11 0
21 0
22 0
J2 6
1 0
2 0
11 0
12 0
22 0
23 0
J3 6
2 0
3 0
12 0
13 0
23 0
24 0
J4 6
3 0
4 0
13 0
14 0
24 0
25 0
J5 6
4 0
5 0
14 0
15 0
25 0
26 0
J6 6
5 0
6 0
15 0
16 0
26 0
27 0
J7 6
6 0
7 0
16 0
17 0
27 0
28 0
J8 6
7 0
8 0
17 0
18 0
28 0
29 0
J9 6
8 0
9 0
18 0
19 0
29 0
30 0
J10 5
9 0
19 0
20 0
30 0
31 0
G0 11
10 445.90909090909093
11 445.90909090909093
12 445.90909090909093
13 445.90909090909093
14 445.90909090909093
15 445.90909090909093
16 445.90909090909093
17 445.90909090909093
18 445.90909090909093
19 445.90909090909093
20 222.95454545454547
