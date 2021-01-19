g3 2 1 0	# problem aircraftb
 5 0 1 0 0	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 5 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 0 5	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 10	# common exprs: b,c,o,c1,o1
V5 4 1	#AX[1]
0 -3.933
1 0.107
2 0.126
4 -9.99
n-4.583
V6 2 1	#AX[2]
1 -0.987
3 -22.95
n1.4185
V7 3 1	#AX[3]
0 0.002
2 -0.235
4 5.67
n-0.09210000000000002
V8 2 1	#AX[4]
1 1
3 -1
n0.008400000000000001
V9 2 1	#AX[5]
2 -1
4 -0.196
n-0.0007100000000000001
V10 0 1	#P1
o54	#sumlist
4
o2	#*
o2	#*
n-0.727
v1	#X[2]
v2	#X[3]
o2	#*
o2	#*
n8.39
v2	#X[3]
v3	#X[4]
o16	#-
o2	#*
o2	#*
n684.4
v3	#X[4]
v4	#X[5]
o2	#*
o2	#*
n63.5
v3	#X[4]
v1	#X[2]
V11 0 1	#P2
o0	# + 
o2	#*
o2	#*
n0.949
v0	#X[1]
v2	#X[3]
o2	#*
o2	#*
n0.173
v0	#X[1]
v4	#X[5]
V12 0 1	#P3
o54	#sumlist
3
o2	#*
o2	#*
n-0.716
v0	#X[1]
v1	#X[2]
o16	#-
o2	#*
o2	#*
n1.578
v0	#X[1]
v3	#X[4]
o2	#*
o2	#*
n1.132
v3	#X[4]
v1	#X[2]
V13 0 1	#P4
o16	#-
o2	#*
v0	#X[1]
v4	#X[5]
V14 0 1	#P5
o2	#*
v0	#X[1]
v3	#X[4]
O0 0	#L2force
o54	#sumlist
5
o5	#^
o0	# + 
v5	#AX[1]
v10	#P1
n2
o5	#^
o0	# + 
v6	#AX[2]
v11	#P2
n2
o5	#^
o0	# + 
v7	#AX[3]
v12	#P3
n2
o5	#^
o0	# + 
v8	#AX[4]
v13	#P4
n2
o5	#^
o0	# + 
v9	#AX[5]
v14	#P5
n2
x5	# initial guess
0 0
1 0
2 0
3 0
4 0
b	#5 bounds (on variables)
0 -1000 1000
0 -1000 1000
0 -1000 1000
0 -1000 1000
0 -1000 1000
k4	#intermediate Jacobian column lengths
0
0
0
0
G0 5
0 0
1 0
2 0
3 0
4 0
