g9 0 1 0 4 20130207 0 4 1 244	# problem hs087
 9 4 1 0 4	# vars, constraints, objectives, ranges, eqns
 4 0	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 3 0 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 18 5	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#e1
o0	# + 
o2	#*
n0.007629045301270999
o2	#*
o2	#*
v0	#x3
v1	#x4
o46	#cos
o1	# - 
n1.48477
v2	#x6
o2	#*
n-0.0006565005618922932
o5	#^
v0	#x3
n2
C1	#e2
o0	# + 
o2	#*
n0.007629045301270999
o2	#*
o2	#*
v0	#x3
v1	#x4
o46	#cos
o0	# + 
n1.48477
v2	#x6
o2	#*
n-0.0006565005618922932
o5	#^
v1	#x4
n2
C2	#e3
o0	# + 
o2	#*
n0.007629045301270999
o2	#*
o2	#*
v0	#x3
v1	#x4
o41	#sin
o0	# + 
n1.48477
v2	#x6
o2	#*
n-0.006895840829735231
o5	#^
v1	#x4
n2
C3	#e4
o0	# + 
o2	#*
n-0.007629045301270999
o2	#*
o2	#*
v0	#x3
v1	#x4
o41	#sin
o1	# - 
n1.48477
v2	#x6
o2	#*
n0.006895840829735231
o5	#^
v0	#x3
n2
O0 0	#obj
n0
x4	# initial guess
0 419.5
1 340.5
2 0.5
3 198.175
r	#4 ranges (rhs's)
4 300
4 0
4 0
4 -200
b	#9 bounds (on variables)
0 340 420
0 340 420
0 0 0.5236
0 -1000 1000
0 0 300
0 0 100
0 0 100
0 0 100
0 0 800
k8	#intermediate Jacobian column lengths
4
8
12
13
14
15
16
17
J0 5
0 0
1 0
2 0
4 1
5 1
J1 6
0 0
1 0
2 0
6 1
7 1
8 1
J2 4
0 0
1 0
2 0
3 1
J3 3
0 0
1 0
2 0
G0 5
4 30
5 31
6 28
7 29
8 30
