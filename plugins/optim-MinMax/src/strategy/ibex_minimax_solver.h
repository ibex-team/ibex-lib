#ifndef __MINIMAX_SOLVER__
#define __MINIMAX_SOLVER__

#include "ibex_light_solver.h"
#include "ibex_x_heap_elem.h"
#include "ibex_Timer.h"

using namespace ibex;
using namespace std;

class minimax_solver {

public:
    Function *objective_function;// objective function
    Ctc* x_ctc; // contractor w.r.t constraint on x
    Ctc* x_ctc_inv; // contractor w.r.t the inverse of constraint on x
    light_solver lsolve;

    /* Constructor*/
    minimax_solver(Function *ofunc,Ctc *x_ctc,Ctc *x_ctc_inv,Ctc *xy_ctc,Ctc *xy_ctc_inv);

    /* Runs a B&B like algorithm
     * arguments: -x_ini: initial x box
     *            -y_ini: initial y box
     *            -prec_x: minimum size of x box
     *            -prec_y: minimum size of y box
     *            -stop_prec: stop criterion
     * */
    void solve(IntervalVector x_ini,IntervalVector y_ini,double prec_x,double prec_y,double stop_prec);
private:
    Heap<y_heap_elem> init_y_heap(const IntervalVector& box);
    Heap<x_heap_elem> init_x_heap(const IntervalVector& box,Heap<y_heap_elem> y_heap_ini);
    double compute_min_prec(const IntervalVector& x_box_ini, const IntervalVector& x_box,const IntervalVector& y_box_ini,double prec_y);
    double choose_nbiter(bool midpoint_eval);
    IntervalVector get_feasible_point(const IntervalVector& x_box);

};


#endif
