g3 0 1 0	# problem cresc4
 6 8 1 0 0	# vars, constraints, objectives, ranges, eqns
 8 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 6 3 3	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 44 3	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1[1]
o54	#sumlist
3
o5	#^
o0	# + 
n-1
o0	# + 
v3	#v1
o2	#*
o2	#*
v1	#a
v0	#d
o46	#cos
v5	#t
n2
o5	#^
o0	# + 
v4	#w1
o2	#*
o2	#*
v1	#a
v0	#d
o41	#sin
v5	#t
n2
o16	#-
o5	#^
o0	# + 
v0	#d
v2	#r
n2
C1	#cons1[2]
o54	#sumlist
3
o5	#^
o0	# + 
v3	#v1
o2	#*
o2	#*
v1	#a
v0	#d
o46	#cos
v5	#t
n2
o5	#^
o0	# + 
n-1
o0	# + 
v4	#w1
o2	#*
o2	#*
v1	#a
v0	#d
o41	#sin
v5	#t
n2
o16	#-
o5	#^
o0	# + 
v0	#d
v2	#r
n2
C2	#cons1[3]
o54	#sumlist
3
o5	#^
o0	# + 
v3	#v1
o2	#*
o2	#*
v1	#a
v0	#d
o46	#cos
v5	#t
n2
o5	#^
o0	# + 
n1
o0	# + 
v4	#w1
o2	#*
o2	#*
v1	#a
v0	#d
o41	#sin
v5	#t
n2
o16	#-
o5	#^
o0	# + 
v0	#d
v2	#r
n2
C3	#cons1[4]
o54	#sumlist
3
o5	#^
o0	# + 
n-0.5
o0	# + 
v3	#v1
o2	#*
o2	#*
v1	#a
v0	#d
o46	#cos
v5	#t
n2
o5	#^
o0	# + 
v4	#w1
o2	#*
o2	#*
v1	#a
v0	#d
o41	#sin
v5	#t
n2
o16	#-
o5	#^
o0	# + 
v0	#d
v2	#r
n2
C4	#cons2[1]
o54	#sumlist
3
o5	#^
o0	# + 
n-1
v3	#v1
n2
o5	#^
v4	#w1
n2
o16	#-
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
C5	#cons2[2]
o54	#sumlist
3
o5	#^
v3	#v1
n2
o5	#^
o0	# + 
n-1
v4	#w1
n2
o16	#-
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
C6	#cons2[3]
o54	#sumlist
3
o5	#^
v3	#v1
n2
o5	#^
o0	# + 
n1
v4	#w1
n2
o16	#-
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
C7	#cons2[4]
o54	#sumlist
3
o5	#^
o0	# + 
n-0.5
v3	#v1
n2
o5	#^
v4	#w1
n2
o16	#-
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
O0 0	#f
o54	#sumlist
3
o2	#*
o5	#^
o0	# + 
v0	#d
v2	#r
n2
o53	#acos
o16	#-
o3	#/
o0	# + 
o1	# - 
o5	#^
o2	#*
v1	#a
v0	#d
n2
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
o5	#^
o0	# + 
v0	#d
v2	#r
n2
o2	#*
o2	#*
o2	#*
n2
o0	# + 
v0	#d
v2	#r
v1	#a
v0	#d
o16	#-
o2	#*
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
o53	#acos
o3	#/
o1	# - 
o0	# + 
o5	#^
o2	#*
v1	#a
v0	#d
n2
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
o5	#^
o0	# + 
v0	#d
v2	#r
n2
o2	#*
o2	#*
o2	#*
n2
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
v1	#a
v0	#d
o2	#*
o2	#*
o2	#*
o0	# + 
v0	#d
v2	#r
v1	#a
v0	#d
o41	#sin
o53	#acos
o16	#-
o3	#/
o0	# + 
o1	# - 
o5	#^
o2	#*
v1	#a
v0	#d
n2
o5	#^
o0	# + 
o2	#*
v1	#a
v0	#d
v2	#r
n2
o5	#^
o0	# + 
v0	#d
v2	#r
n2
o2	#*
o2	#*
o2	#*
n2
o0	# + 
v0	#d
v2	#r
v1	#a
v0	#d
x6	# initial guess
0 1
1 2
2 0.75
3 -40
4 5
5 1.5
r	#8 ranges (rhs's)
1 0
1 0
1 0
1 0
2 0
2 0
2 0
2 0
b	#6 bounds (on variables)
0 1e-08 1.e8
0 1  1.e8
0 0.39 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 0 6.2831852
k5	#intermediate Jacobian column lengths
8
16
24
32
40
J0 6
0 0
1 0
2 0
3 0
4 0
5 0
J1 6
0 0
1 0
2 0
3 0
4 0
5 0
J2 6
0 0
1 0
2 0
3 0
4 0
5 0
J3 6
0 0
1 0
2 0
3 0
4 0
5 0
J4 5
0 0
1 0
2 0
3 0
4 0
J5 5
0 0
1 0
2 0
3 0
4 0
J6 5
0 0
1 0
2 0
3 0
4 0
J7 5
0 0
1 0
2 0
3 0
4 0
G0 3
0 0
1 0
2 0
