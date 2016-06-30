#ifndef __LIGHT_SOLVER__
#define __LIGHT_SOLVER__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "ibex_y_heap_elem.h"
#include "ibex_Heap.h"
#include "ibex_Ctc.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_LargestFirst.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_DoubleHeap.h"


using namespace std;
using namespace ibex;

class light_solver{
public:
    Ctc * ctc_xy; //contractor for constraints on xy
    NormalizedSystem *y_sys; // contains constraints on x and y
    double abs_min_prec; // absolute minimum prec bissection on y

    /* Constructor*/
    light_solver(Ctc * ctc_xy,NormalizedSystem *y_sys);

    /* returns an enclosure of the maximum of the objective function: max f(x,y)
     * modifies y_heap inherited from father box
     * This function works as a classic B&B algorithm, with an initial stack of box instead of a initial box.
     * Inputs: -y_heap: heap of cells containing y boxes sorted w.r.t the ub of eval, the top cell has the greatest ub eval.
     *         -x_box: current x box, needed to evaluate f(x,y)
     *         -objective_function: f
     *         -nb_iter: number of times boxes of y_heap are cut
     *         -best_max: minimum found for max(f), best current solution
     *         -fmax: enclosure of max(f(x_box),y_heap), this result is inherited from early computation with a box containing x_box
     *         -min_prec: minimum size of boxes in y_heap
     *         -is_midp: true if optimize run with x midpoint eval, false else
     * */
    Interval optimize(DoubleHeap<y_heap_elem> * y_heap, IntervalVector *x_box,Function * objective_function, unsigned nb_iter,double best_max,Interval fmax, double min_prec,bool is_midp);

    /* contract xy_box and xy_box_ctc w.r.t max_ctc contractor
     * */
    void contract_best_max_cst( Ctc* max_ctc,IntervalVector* xy_box,IntervalVector* xy_box_ctc,y_heap_elem* elem);

    /* return a feasible point in y_box w.r.t constraints on xy
     *  */
    IntervalVector get_feasible_point(const IntervalVector& xy_box,y_heap_elem * elem);

    /* return 0 if box is non feasible w.r.t constraints on xy, 1 if not known, 2 if box is entierly feasible
     * */
    int check_constraints(const IntervalVector& box);
};

/* returns a box composed of x_box(not modified) and the middle of y_box, needed for midpoint evaluation
 * Inputs: -xy_box: whole box
 *         -y_box: y box to get the middle
 */
IntervalVector get_mid_y(const IntervalVector& xy_box,const IntervalVector& y_box);

/* set y part of xy_box with y_box
 * */
void init_xy_box(IntervalVector* xy_box,const IntervalVector & y_box);

/* add elements of Heap_save into y_heap
 * */
void fill_y_heap(DoubleHeap<y_heap_elem>* y_heap,vector<y_heap_elem*>* Heap_save);

#endif
