g9 0 1 0 15 20130207 0 4 0 240	# problem avion2
 49 15 1 0 15	# vars, constraints, objectives, ranges, eqns
 0 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 0 37 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 43 37	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons1
n0
C1	#cons2
n0
C2	#cons3
n0
C3	#cons5
n0
C4	#cons11
n0
C5	#cons12
n0
C6	#cons15
n0
C7	#cons17
n0
C8	#cons19
n0
C9	#cons20
n0
C10	#cons21
n0
C11	#cons22
n0
C12	#cons23
n0
C13	#cons24
n0
C14	#cons25
n0
O0 0	#f
o54	#sumlist
17
o5	#^
o1	# - 
v6	#SK
o2	#*
o2	#*
n0.01
v1	#PK
v0	#SR
n2
o5	#^
o1	# - 
v10	#CA
o3	#/
o1	# - 
o1	# - 
v13	#SS
v12	#SO
o2	#*
v11	#CB
v8	#LF
o5	#^
v8	#LF
n2
n2
o5	#^
o54	#sumlist
4
o2	#*
n-2
v9	#AM
v12	#SO
v13	#SS
o3	#/
o2	#*
n0.01
v2	#EF
v8	#LF
n2
o5	#^
o1	# - 
v9	#AM
o3	#/
o2	#*
o2	#*
n0.025
v12	#SO
o5	#^
v11	#CB
n2
v10	#CA
n2
o5	#^
o54	#sumlist
3
v14	#IMPDER
o2	#*
n-27.5
v5	#SD
o2	#*
n-1.3
o5	#^
v5	#SD
n2
n2
o5	#^
o54	#sumlist
3
v15	#IMPK
o2	#*
n-70
v6	#SK
o2	#*
n8.6
o5	#^
v6	#SK
n2
n2
o5	#^
o54	#sumlist
3
n-1000
v16	#QI
o3	#/
o5	#^
v18	#MV
n2
n24000
n2
o5	#^
o1	# - 
o2	#*
n1000
v17	#PT
o2	#*
v20	#MD
v21	#PD
n2
o5	#^
o0	# + 
o1	# - 
o54	#sumlist
4
n2
v27	#VN
v22	#VS
o3	#/
v29	#QF
n790
o3	#/
v26	#MZ
v23	#CR
o2	#*
v25	#DV
v17	#PT
n2
o5	#^
o0	# + 
o1	# - 
v30	#IMPMOT
o3	#/
o2	#*
n1000
v17	#PT
o0	# + 
n20
v24	#PM
o2	#*
n-12
o39	#sqrt
v17	#PT
n2
o5	#^
o1	# - 
v7	#ST
o2	#*
o2	#*
n1.25
v0	#SR
v36	#NM
n2
o5	#^
o1	# - 
v0	#SR
o3	#/
v20	#MD
v31	#MS
n2
o5	#^
o1	# - 
v28	#QV
o3	#/
o2	#*
o2	#*
o2	#*
n2.4
v3	#SX
o39	#sqrt
v3	#SX
v32	#EL
o39	#sqrt
v4	#LX
n2
o5	#^
o1	# - 
v12	#SO
o2	#*
o2	#*
n0.785
o5	#^
v33	#DE
n2
v17	#PT
n2
o5	#^
o1	# - 
v13	#SS
o2	#*
o2	#*
n0.785
o5	#^
v34	#DS
n2
v17	#PT
n2
o5	#^
o1	# - 
v11	#CB
o3	#/
o2	#*
n2
o1	# - 
v27	#VN
o2	#*
v10	#CA
o5	#^
v8	#LF
n3
o2	#*
o5	#^
v8	#LF
n2
o1	# - 
n3
o2	#*
v12	#SO
v8	#LF
n2
o5	#^
o1	# - 
v35	#IMPVOIL
o2	#*
o2	#*
o2	#*
n1.15
v3	#SX
o0	# + 
n15
o2	#*
n0.15
v3	#SX
o0	# + 
n8
o5	#^
o3	#/
o2	#*
v19	#MC
v4	#LX
o2	#*
o2	#*
n50
v0	#SR
v32	#EL
n1.5
n2
x49	# initial guess
0 27.452
1 10
2 0
3 19.217
4 1.5
5 3.5688
6 4.0696
7 34.315
8 5.1306
9 0
10 -0.14809
11 0.7598
12 0
13 0
14 114.7
15 500
16 2325.6
17 5.6788
18 14197
19 12589
20 28394
21 0.2
22 0
23 100
24 15
25 0
26 500
27 10
28 814.9
29 3140.5
30 190.85
31 1000
32 4.9367
33 0
34 0
35 5000
36 1
37 1.5
38 88.025
39 1760.5
40 1
41 1945
42 35
43 100
44 200
45 120
46 700
47 1
48 1
r	#15 ranges (rhs's)
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
4 400
4 380
4 -290
b	#49 bounds (on variables)
0 10 150
0 0 10
0 0 5
0 7 120
0 1.5 8
0 2 20
0 2 30
0 30 500
0 0.01 20
0 0 10
0 -0.2 -0.001
0 0.1 2
0 0 1
0 0 2
0 100 1000
0 500 5000
0 1000 20000
0 2 30
0 2000 20000
0 3000 30000
0 5000 50000
0 0.2 0.8
0 0 20
0 100 400
0 4 15
0 0 10
0 500 10000
0 10 50
0 250 5000
0 750 15000
0 10 5000
0 100 1000
0 2 20
0 0 1
0 0 2
0 500 5000
0 1 2
0 0 10
0 20 200
0 500 5000
0 1 5
0 250 3000
0 35 70
0 100 3000
0 200 400
0 120 240
0 700 1900
0 1 2
0 1 2
k48	#intermediate Jacobian column lengths
2
2
2
4
5
7
8
9
9
9
9
9
9
9
9
9
12
12
15
16
17
17
17
17
17
17
18
18
19
20
20
20
20
20
20
20
22
23
25
26
27
29
31
33
35
37
39
41
J0 2
0 -0.13
5 1
J1 2
0 -0.7
3 1
J2 2
4 1
37 -1
J3 5
3 -2
5 -2
6 -2
7 -1
38 1
J4 2
38 -20
39 1
J5 2
18 -2
20 1
J6 3
16 -1
28 -1
29 1
J7 2
18 -0.137
41 1
J8 2
36 -35
42 1
J9 2
16 -0.043
43 1
J10 2
44 1
47 -200
J11 2
45 1
48 -120
J12 2
40 -300
46 1
J13 6
16 0.5
18 -1
19 1
36 660
47 95
48 70
J14 7
26 1
41 -1
42 1
43 1
44 1
45 1
46 1
G0 37
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
28 0
29 0
30 0
31 0
32 0
33 0
34 0
35 0
36 0
