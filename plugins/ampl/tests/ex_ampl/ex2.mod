var x{0..2};
var y;

minimize goal: y-cos(x[1]);

s.t. con1: x[0]+x[1]>=0;
s.t. con2: x[1]+x[2]<=y;
s.t. con3: y >= x[0];



write gex2;
