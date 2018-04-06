#include "ibex.h"

#include <stdlib.h>
using namespace std;
using namespace ibex;
int main(int argc, char** argv) {

Variable v(2);
Function f (v, 2*v[0]+ 4*v[1]);
IntervalVector b0(2);
b0[0]= Interval(0,1);
b0[1]= Interval(0,1);

Affine2Eval eval_af(f);
 Interval affine0 = eval_af.eval(b0).i();
Affine2  res_affine = eval_af.af2.top->i();
 cout << b0 << " " << affine0 << " " << res_affine.mid() << " " << res_affine.val(1) << " " << res_affine.val(2) << endl;

 affine0 =  eval_af.eval(b0).i();

cout << b0 << " " << affine0 << " " << res_affine.mid() << " " << res_affine.val(1) << " " << res_affine.val(2) << endl;

}
