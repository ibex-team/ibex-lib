var x1 >= 1, <= 16;
var x2 >= 1, <= 16;
var x3 >= 1, <= 16;
var x4 >= 1, <= 16;
var x6 >= 1, <= 1000;
var x7 >= 0.0001, <= 1.e8;
var x8 >= 0.0001, <= 1.e8;
var x9 >= 1, <= 1.e8;
var x10 >= -1.e8, <= 50;
var x11 >= 100, <= 1.e8;
var x12 >= 1, <= 1.e8;
var x13 >= 0.0001, <= 1.e8;
var x14 >= 0.01, <= 1.e8;



minimize goal: x7 + x8;

subject to con1 : - 1.42857142857143*x4*x6 + 10000*x8 =0;
subject to con2 : 10*x7*x9 - 0.00968946189201592*x3*(x1**4 - x2**4) =0;
subject to con3 : 143.3076*x10*x4 - 10000*x7 =0;
subject to con4 : 3.1415927*x6*(0.001*x9)**3 - 6e-6*x3*x4*x13 =0;
subject to con5 : 101000*x12*x13 - 1.57079635*x6*x14 =0;
subject to con6 : log(0.8 + 8.112*x3)/log(10) - 10964781961.4318*exp(log(x11)*(-3.55)) =0;
subject to con7 : - 0.5*x10 + x11 - 560 =0;
subject to con8 : x1 - x2 >= 0;
subject to con9 : 0.0307*x4**2 - 0.3864*(0.0062831854*x1*x9)**2*x6 <= 0;
subject to con10 : 101000*x12 - 15707.9635*x14 <= 0;
subject to con11 : - (log(x1) - log(x2)) + x13 =0;
subject to con12 : - (x1^2 - x2^2) + x14 =0;



write gbearing; 
