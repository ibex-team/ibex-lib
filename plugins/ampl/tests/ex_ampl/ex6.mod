var x;
var y;
var z1 = x+ y ;
var z2 = z1*z1+ y ;
var z3 = z1*z1+ z2 -x ;
var z4 = z3*z2+ z1*z3 -x ;

minimize goal: z4;

s.t. con1: x+y <= 0;
s.t. con2: z2-y <= 0;
s.t. con3: x-z3 >= 0;

write gex6;

