g3 0 1 0	# problem disc2
 28 23 1 0 17	# vars, constraints, objectives, ranges, eqns
 23 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 28 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 81 1	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#b1
o54	#sumlist
3
o5	#^
v1	#u1
n2
o5	#^
o0	# + 
n-10
v2	#v1
n2
o16	#-
o5	#^
v0	#epsilon
n2
C1	#b2
o54	#sumlist
3
o5	#^
o0	# + 
n-8
v3	#u2
n2
o5	#^
o0	# + 
n-10
v4	#v2
n2
o16	#-
o5	#^
v0	#epsilon
n2
C2	#b3
o54	#sumlist
3
o5	#^
o0	# + 
n-12
v5	#u3
n2
o5	#^
o0	# + 
n-5
v6	#v3
n2
o16	#-
o5	#^
v0	#epsilon
n2
C3	#b4
o54	#sumlist
3
o5	#^
o0	# + 
n-8
v7	#u4
n2
o5	#^
v8	#v4
n2
o16	#-
o5	#^
v0	#epsilon
n2
C4	#b5
o54	#sumlist
3
o5	#^
v9	#u5
n2
o5	#^
v10	#v5
n2
o16	#-
o5	#^
v0	#epsilon
n2
C5	#b6
o54	#sumlist
3
o16	#-
o5	#^
o0	# + 
n-4
v11	#u6
n2
o16	#-
o5	#^
o0	# + 
n-8
v12	#v6
n2
o5	#^
v0	#epsilon
n2
C6	#b7
o54	#sumlist
3
o16	#-
o5	#^
o0	# + 
n-8
v13	#u7
n2
o16	#-
o5	#^
o0	# + 
n-7
v14	#v7
n2
o5	#^
v0	#epsilon
n2
C7	#b8
o54	#sumlist
3
o16	#-
o5	#^
o0	# + 
n-8
v15	#u8
n2
o16	#-
o5	#^
o0	# + 
n-3
v16	#v8
n2
o5	#^
v0	#epsilon
n2
C8	#b9
o54	#sumlist
3
o16	#-
o5	#^
o0	# + 
n-4
v17	#u9
n2
o16	#-
o5	#^
o0	# + 
n-1
v18	#v9
n2
o5	#^
v0	#epsilon
n2
C9	#b10
o54	#sumlist
3
o16	#-
o5	#^
o0	# + 
n-2
v19	#u10
n2
o16	#-
o5	#^
o0	# + 
n-3
v20	#v10
n2
o5	#^
v0	#epsilon
n2
C10	#b11
o54	#sumlist
3
o16	#-
o5	#^
o0	# + 
n-2
v21	#u11
n2
o16	#-
o5	#^
o0	# + 
n-6
v22	#v11
n2
o5	#^
v0	#epsilon
n2
C11	#b162
o16	#-
o2	#*
o1	# - 
v3	#u2
v1	#u1
v23	#alpha1
C12	#c162
o16	#-
o2	#*
o1	# - 
v4	#v2
v2	#v1
v23	#alpha1
C13	#b273
o16	#-
o2	#*
o1	# - 
v5	#u3
v3	#u2
v24	#alpha2
C14	#c273
o16	#-
o2	#*
o1	# - 
v6	#v3
v4	#v2
v24	#alpha2
C15	#b384
o16	#-
o2	#*
o1	# - 
v7	#u4
v5	#u3
v25	#alpha3
C16	#c384
o16	#-
o2	#*
o1	# - 
v8	#v4
v6	#v3
v25	#alpha3
C17	#b495
o16	#-
o2	#*
o1	# - 
v9	#u5
v7	#u4
v26	#alpha4
C18	#c495
o16	#-
o2	#*
o1	# - 
v10	#v5
v8	#v4
v26	#alpha4
C19	#b5101
o16	#-
o2	#*
o1	# - 
v1	#u1
v9	#u5
v27	#alpha5
C20	#c5101
o16	#-
o2	#*
o1	# - 
v2	#v1
v10	#v5
v27	#alpha5
C21	#b5111
o16	#-
o2	#*
o1	# - 
v1	#u1
v9	#u5
v27	#alpha5
C22	#c5111
o16	#-
o2	#*
o1	# - 
v2	#v1
v10	#v5
v27	#alpha5
O0 0	#obj
n0
x28	# initial guess
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
20 0
21 0
22 0
23 0
24 0
25 0
26 0
27 0
r	#23 ranges (rhs's)
4 0
4 0
4 0
4 0
4 0
2 0
2 0
2 0
2 0
2 0
2 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
b	#28 bounds (on variables)
0 0 3
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
0 0 1
0 0 1
0 0 1
0 0 1
0 0 1
k27	#intermediate Jacobian column lengths
11
15
19
22
25
28
31
34
37
41
45
47
49
51
53
55
57
59
61
63
65
67
69
71
73
75
77
J0 3
0 0
1 0
2 0
J1 3
0 0
3 0
4 0
J2 3
0 0
5 0
6 0
J3 3
0 0
7 0
8 0
J4 3
0 0
9 0
10 0
J5 3
0 0
11 0
12 0
J6 3
0 0
13 0
14 0
J7 3
0 0
15 0
16 0
J8 3
0 0
17 0
18 0
J9 3
0 0
19 0
20 0
J10 3
0 0
21 0
22 0
J11 4
1 -1
3 0
11 1
23 0
J12 4
2 -1
4 0
12 1
23 0
J13 4
3 -1
5 0
13 1
24 0
J14 4
4 -1
6 0
14 1
24 0
J15 4
5 -1
7 0
15 1
25 0
J16 4
6 -1
8 0
16 1
25 0
J17 4
7 -1
9 0
17 1
26 0
J18 4
8 -1
10 0
18 1
26 0
J19 4
1 0
9 -1
19 1
27 0
J20 4
2 0
10 -1
20 1
27 0
J21 4
1 0
9 -1
21 1
27 0
J22 4
2 0
10 -1
22 1
27 0
G0 1
0 1
