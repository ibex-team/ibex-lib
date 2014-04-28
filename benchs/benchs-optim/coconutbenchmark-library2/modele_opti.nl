g9 3 1 0 3 20130207 0 4 0 500	# problem modele_opti
 5 3 1 0 2	# vars, constraints, objectives, ranges, eqns
 2 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 3 5 3	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 8 5	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 4 0 0 0 8	# common exprs: b,c,o,c1,o1
V5 1 0	#k1
2 0.0019416364259847942
n0
V6 1 0	#k2
2 0.0016161174665886159
n0
V7 0 0	#X1
o2	#*
v5	#k1
v0	#L1
V8 0 0	#X2
o2	#*
v6	#k2
v1	#L2
C0	#=contrainte
o1	# - 
o16	#-
o2	#*
o46	#cos
v7	#X1
o41	#sin
v8	#X2
o2	#*
o2	#*
n0.8323481394148877
o41	#sin
v7	#X1
o46	#cos
v8	#X2
C1	#=cont2
o1	# - 
o2	#*
n-0.8323481394148877
o38	#tan
v7	#X1
o38	#tan
v8	#X2
C2	#const2
n0
V9 0 4	#beta
o54	#sumlist
3
o46	#cos
o2	#*
v6	#k2
v3	#y
o2	#*
o38	#tan
v8	#X2
o41	#sin
o2	#*
v6	#k2
v3	#y
n-1
V10 0 4	#gamm
o0	# + 
o2	#*
o3	#/
v8	#X2
o5	#^
o46	#cos
v8	#X2
n2
o41	#sin
o2	#*
v6	#k2
v3	#y
o2	#*
o2	#*
v6	#k2
v3	#y
o1	# - 
o2	#*
o38	#tan
v8	#X2
o46	#cos
o2	#*
v6	#k2
v3	#y
o41	#sin
o2	#*
v6	#k2
v3	#y
V11 0 4	#alph
o2	#*
o2	#*
n114875517522.18855
v6	#k2
o1	# - 
o2	#*
o2	#*
o0	# + 
n0.8323481394148877
v7	#X1
o41	#sin
v7	#X1
o41	#sin
v8	#X2
o2	#*
o2	#*
o0	# + 
o2	#*
n0.8323481394148877
v7	#X1
v8	#X2
o46	#cos
v7	#X1
o46	#cos
v8	#X2
V12 0 4	#epsi1
o3	#/
o2	#*
n16
o2	#*
o2	#*
n-17672669820.33213
o1	# - 
n1
o46	#cos
v7	#X1
o46	#cos
v8	#X2
v4	#e
V13 0 4	#epsi2
o3	#/
o2	#*
n-282762717125.3141
o1	# - 
o2	#*
o2	#*
v7	#X1
o41	#sin
v7	#X1
o46	#cos
v8	#X2
o2	#*
o2	#*
v8	#X2
o1	# - 
n1
o46	#cos
v7	#X1
o41	#sin
v8	#X2
v4	#e
V14 0 4	#imag_phi
o3	#/
o2	#*
n600
o2	#*
v12	#epsi1
v9	#beta
v11	#alph
V15 0 4	#real_phi
o2	#*
o3	#/
n1
v11	#alph
o0	# + 
o2	#*
v13	#epsi2
v9	#beta
o2	#*
v12	#epsi1
v10	#gamm
V16 0 4	#modul_phi
o39	#sqrt
o0	# + 
o5	#^
v15	#real_phi
n2
o5	#^
v14	#imag_phi
n2
O0 1	#f
v16	#modul_phi
x5	# initial guess
0 0.0125
1 0.0125
2 70063.4738882004
3 0.0125
4 0.0017
r	#3 ranges (rhs's)
4 0
4 0
1 0
b	#5 bounds (on variables)
0 0.002 0.201
0 0.002 0.202
0 100 5e+05
0 0.002 0.203
0 0.001 0.01
k4	#intermediate Jacobian column lengths
2
5
7
8
J0 3
0 0
1 0
2 0
J1 3
0 0
1 0
2 0
J2 2
1 -1
3 1
G0 5
0 0
1 0
2 0
3 0
4 0
