var x{0..2};
var A{0..2,0..2};
var y >= -1, <=1;

param a{0..2};

let a[0]:=10;
let a[1]:=11;
let a[2]:=12;

minimize goal: y-exp(x[1]);

s.t. con1{i in 0..2} :  sum {j in 0..2} A[i,j] * x[j] = a[i];
s.t. con2: y >= x[0];

write gex1;

