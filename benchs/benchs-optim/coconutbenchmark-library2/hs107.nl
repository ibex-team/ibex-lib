g3 1 1 0	# problem hs107
 9 6 1 0 6	# vars, constraints, objectives, ranges, eqns
 6 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 5 7 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 34 2	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 6 0 0 0	# common exprs: b,c,o,c1,o1
V9 0 0	#y1
o41	#sin
v3	#x[8]
V10 0 0	#y2
o46	#cos
v3	#x[8]
V11 0 0	#y3
o41	#sin
v4	#x[9]
V12 0 0	#y4
o46	#cos
v4	#x[9]
V13 0 0	#y5
o41	#sin
o1	# - 
v3	#x[8]
v4	#x[9]
V14 0 0	#y6
o46	#cos
o1	# - 
v3	#x[8]
v4	#x[9]
C0	#c1
o54	#sumlist
3
o2	#*
n0.47729399027100244
o5	#^
v0	#x[5]
n2
o16	#-
o2	#*
o2	#*
v0	#x[5]
v1	#x[6]
o0	# + 
o2	#*
n0.9346173710697385
v9	#y1
o2	#*
n0.23864699513550122
v10	#y2
o16	#-
o2	#*
o2	#*
v0	#x[5]
v2	#x[7]
o0	# + 
o2	#*
n0.9346173710697385
v11	#y3
o2	#*
n0.23864699513550122
v12	#y4
C1	#c2
o54	#sumlist
3
o2	#*
n0.47729399027100244
o5	#^
v1	#x[6]
n2
o2	#*
o2	#*
v0	#x[5]
v1	#x[6]
o0	# + 
o2	#*
n0.9346173710697385
v9	#y1
o2	#*
n-0.23864699513550122
v10	#y2
o2	#*
o2	#*
v1	#x[6]
v2	#x[7]
o0	# + 
o2	#*
n0.9346173710697385
v13	#y5
o2	#*
n-0.23864699513550122
v14	#y6
C2	#c3
o54	#sumlist
3
o2	#*
n0.47729399027100244
o5	#^
v2	#x[7]
n2
o2	#*
o2	#*
v0	#x[5]
v2	#x[7]
o0	# + 
o2	#*
n0.9346173710697385
v11	#y3
o2	#*
n-0.23864699513550122
v12	#y4
o16	#-
o2	#*
o2	#*
v1	#x[6]
v2	#x[7]
o0	# + 
o2	#*
n0.9346173710697385
v13	#y5
o2	#*
n0.23864699513550122
v14	#y6
C3	#c4
o54	#sumlist
3
o2	#*
n1.869234742139477
o5	#^
v0	#x[5]
n2
o2	#*
o2	#*
v0	#x[5]
v1	#x[6]
o0	# + 
o2	#*
n0.23864699513550122
v9	#y1
o2	#*
n-0.9346173710697385
v10	#y2
o2	#*
o2	#*
v0	#x[5]
v2	#x[7]
o0	# + 
o2	#*
n0.23864699513550122
v11	#y3
o2	#*
n-0.9346173710697385
v12	#y4
C4	#c5
o54	#sumlist
3
o2	#*
n1.869234742139477
o5	#^
v1	#x[6]
n2
o16	#-
o2	#*
o2	#*
v0	#x[5]
v1	#x[6]
o0	# + 
o2	#*
n0.23864699513550122
v9	#y1
o2	#*
n0.9346173710697385
v10	#y2
o16	#-
o2	#*
o2	#*
v1	#x[6]
v2	#x[7]
o0	# + 
o2	#*
n0.23864699513550122
v13	#y5
o2	#*
n0.9346173710697385
v14	#y6
C5	#c6
o54	#sumlist
3
o2	#*
n1.869234742139477
o5	#^
v2	#x[7]
n2
o16	#-
o2	#*
o2	#*
v0	#x[5]
v2	#x[7]
o0	# + 
o2	#*
n0.23864699513550122
v11	#y3
o2	#*
n0.9346173710697385
v12	#y4
o2	#*
o2	#*
v1	#x[6]
v2	#x[7]
o0	# + 
o2	#*
n0.23864699513550122
v13	#y5
o2	#*
n-0.9346173710697385
v14	#y6
O0 0	#obj
o0	# + 
o2	#*
n1000
o5	#^
v5	#x[1]
n3
o2	#*
n666.667
o5	#^
v6	#x[2]
n3
x8	# initial guess
0 1.0454
1 1.0454
2 0
3 0
5 0.8
6 0.8
7 0.2
8 0.2
r	#6 ranges (rhs's)
4 -0.4
4 -0.4
4 -0.8
4 -0.2
4 -0.2
4 0.337
b	#9 bounds (on variables)
0 0.90909 1.0909
0 0.90909 1.0909
0 0.90909 1.0909
0 -1.e8 1.e8
0 -1.e8 1.e8
0 0 1.e8
0 0 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k8	#intermediate Jacobian column lengths
6
12
18
24
30
31
32
33
J0 6
0 0
1 0
2 0
3 0
4 0
5 -1
J1 6
0 0
1 0
2 0
3 0
4 0
6 -1
J2 5
0 0
1 0
2 0
3 0
4 0
J3 6
0 0
1 0
2 0
3 0
4 0
7 -1
J4 6
0 0
1 0
2 0
3 0
4 0
8 -1
J5 5
0 0
1 0
2 0
3 0
4 0
G0 2
5 3000
6 2000
