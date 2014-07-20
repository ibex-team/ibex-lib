#include "ibex_CtcPointInCircle.h"
namespace ibex {


CtcPointInCircle::CtcPointInCircle(double cx, double cy, double r) : Ctc(), _cx(cx), _cy(cy), _r(r)

{
    Variable x(2),c(2),radius;

    Function *f = new Function(x,c,radius,sqr(x[0]-c[0]) + sqr(x[1]-c[1]) - sqr(radius));

    
    ncf = new NumConstraint(*f,EQ,true);
    
    ctc_f = new CtcFwdBwd(*ncf);
    
//    X_with_params[2] = Interval(cx);
//    X_with_params[3] = Interval(cy);
//    X_with_params[4] = Interval(r);
    
}

CtcPointInCircle::~CtcPointInCircle(){
    delete ncf; 
    delete ctc_f;
}
void CtcPointInCircle::contract(IntervalVector &box, double cx, double cy, double r){

    IntervalVector X_with_params(2+3);
    X_with_params[2] = Interval(cx);
    X_with_params[3] = Interval(cy);
    X_with_params[4] = Interval(r);
    X_with_params[0] = box[0];
    X_with_params[1] = box[1];
    try {
        ctc_f->contract(X_with_params);
        box[0] = X_with_params[0];
        box[1] = X_with_params[1];
    } catch(EmptyBoxException& e) {
        box.set_empty();
        throw e;
    }
}

void CtcPointInCircle::contract(IntervalVector &box)
{
//     try {
// //        cout << X_with_params << " " << X_with_params.is_empty() << endl;
// //        X_with_params.put(0,box);
//          X_with_params[0] = box[0];
//          X_with_params[1] = box[1];
//          // X_with_params[2] = Interval(_cx);
//          // X_with_params[3] = Interval(_cy);
//          // X_with_params[4] = Interval(_r);

// //        X_with_params=cart_prod(box,X_with_params.subvector(2,4));
//         ctc_f->contract(X_with_params);
// //        box = X_with_params.subvector(0,1);
//         box[0] = X_with_params[0];
//         box[1] = X_with_params[1];
//     } catch(EmptyBoxException& e) {
//         box.set_empty();
// //        X_with_params = IntervalVector(5);
//         throw e;
//     }
}

}


