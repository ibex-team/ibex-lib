#include "ibex.h"

#include <stdlib.h>
using namespace std;
using namespace ibex;
int main(int argc, char** argv) {

Variable v(2);
 Function f (v, v[0]+ v[1] /v[0]);
IntervalVector b0(2);
b0[0]= Interval(0,1);
b0[1]= Interval(0,1);

Affine2Eval eval_af(f);
 Interval affine0 = eval_af.eval(b0).i();
 Interval eval0 = f.eval(b0);
Affine2  res_affine = eval_af.af2.top->i();
 cout << " eval naturelle" << eval0 << endl;
 cout << " eval affine " << " " << affine0 << endl;

}
