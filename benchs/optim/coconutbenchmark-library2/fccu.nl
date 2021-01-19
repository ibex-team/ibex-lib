g3 0 1 0	# problem fccu
 19 8 1 0 8	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 19 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 28 19	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
n0
C1	#cons2
n0
C2	#cons3
n0
C3	#cons4
n0
C4	#cons5
n0
C5	#cons6
n0
C6	#cons7
n0
C7	#cons8
n0
O0 0	#f
o54	#sumlist
19
o2	#*
n5
o5	#^
o0	# + 
n-31
v0	#Feed
n2
o5	#^
o0	# + 
n-36
v1	#Effluent
n2
o5	#^
o0	# + 
n-20
v2	#MF_ohd
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-3
v3	#HCN
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-5
v4	#LCO
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-3.5
v5	#HCO
n2
o5	#^
o0	# + 
n-4.2
v6	#MF_btms
n2
o5	#^
o0	# + 
n-0.9
v7	#Decant
n2
o5	#^
o0	# + 
n-3.9
v8	#Dec_recy
n2
o5	#^
o0	# + 
n-2.2
v9	#Off_gas
n2
o5	#^
o0	# + 
n-22.8
v10	#DC4_feed
n2
o5	#^
o0	# + 
n-6.8
v11	#DC3_feed
n2
o5	#^
o0	# + 
n-19
v12	#DC4_btms
n2
o5	#^
o0	# + 
n-8.5
v13	#Lean_oil
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-2.2
v14	#Propane
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-2.5
v15	#Butane
n2
o5	#^
o0	# + 
n-10.8
v16	#C8spl_fd
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-6.5
v17	#LCN
n2
o2	#*
n3.0000000300000003
o5	#^
o0	# + 
n-6.5
v18	#MCN
n2
x19	# initial guess
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
r	#8 ranges (rhs's)
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
b	#19 bounds (on variables)
3
3
3
3
3
3
3
3
3
3
3
3
3
3
3
3
3
3
3
k18	#intermediate Jacobian column lengths
1
3
5
6
7
8
10
11
13
14
16
18
20
22
23
24
26
27
J0 3
0 1
1 -1
8 1
J1 6
1 1
2 -1
3 -1
4 -1
5 -1
6 -1
J2 3
6 1
7 -1
8 -1
J3 4
2 1
9 -1
10 -1
13 1
J4 3
10 1
11 -1
12 -1
J5 3
12 1
13 -1
16 -1
J6 3
11 1
14 -1
15 -1
J7 3
16 1
17 -1
18 -1
G0 19
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
