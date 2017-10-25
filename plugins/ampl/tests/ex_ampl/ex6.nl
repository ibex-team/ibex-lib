g3 0 1 0	# problem ex6
 2 3 1 0 0	# vars, constraints, objectives, ranges, eqns
 2 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 2 2 2	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 6 2	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 3 0 0 0 3	# common exprs: b,c,o,c1,o1
b
3
3
r
1 0
2 0
1 0
V2 2 0
0 1
1 1
n0
V3 0 0
o2
v2
v2
V4 0 0
o0
v3
o2
v2
v2
C0
v3
C1
o16
v4
C2
n0
V5 1 4
1 1
v3
V6 2 4
0 -1
1 1
v4
V7 0 4
o0
o2
v6
v5
o2
v2
v6
O0 0
v7
k1
3
J0 2
0 0
1 0
J1 2
0 2
1 -1
J2 2
0 1
1 1
G0 2
0 -1
1 0
