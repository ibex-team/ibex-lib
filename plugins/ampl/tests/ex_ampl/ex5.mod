var x;
var y;
var z = x+ y ;
var zz = z*z+ y ;

minimize goal: zz;

s.t. con1: x+y = 0;
s.t. con2: zz-y <= 0;
s.t. con3: x-z >= 0;



write gex5;
