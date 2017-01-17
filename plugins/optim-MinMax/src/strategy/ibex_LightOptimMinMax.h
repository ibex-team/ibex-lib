//============================================================================
//                                  I B E X
// File        : ibex_LightOptimMinMax.h
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================


#ifndef __LIGHT_MINMAX__
#define __LIGHT_MINMAX__


#include <vector>
#include "ibex_Ctc.h"
#include "ibex_Bsc.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_Cell.h"
#include "ibex_DoubleHeap.h"
#include "ibex_OptimData.h"

namespace ibex {

class LightOptimMinMax {
public:

    /* Constructor*/
    LightOptimMinMax(NormalizedSystem& y_sys,Ctc& ctc_xy);

    /* Destructor */
    ~LightOptimMinMax();
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
    bool optimize(Cell* x_cell, int nb_iter, double prec_y);

	/**
	 * Allows to add the backtrackable data required
	 * by this MinMax optimizer to the root cell
	 */
    void add_backtrackable(Cell& root, const IntervalVector& y_init);



    int trace;
    double timeout;

private:
    Ctc& ctc_xy; //contractor for constraints on xy
    NormalizedSystem& xy_sys; // contains constraints on x and y
    //double abs_min_prec; // absolute minimum prec bissection on y
    Bsc* bsc; // bissector
    std::vector<Cell*> heap_save;
    double prec_y;
    bool found_point;
    double time;
    /* contract xy_box and xy_box_ctc w.r.t max_ctc contractor
     * */
//    void contract_best_max_cst( Ctc* max_ctc,IntervalVector* xy_box,IntervalVector* xy_box_ctc,y_heap_elem* elem);

    /* return a feasible point in y_box w.r.t constraints on xy
     *  */
    IntervalVector get_feasible_point(Cell* x_cell, Cell * y_cell);

    /* return 0 if box is non feasible w.r.t constraints on xy, 1 if not known, 2 if box is entierly feasible
     * */
    int check_constraints(const IntervalVector& xy_box);

    bool handle_cstfree(IntervalVector * xy_box);

    bool handle_cell( Cell* x_cell, Cell* y_cell, bool cst);

    bool handle_constraint(OptimData  *data_y, IntervalVector * xy_box,IntervalVector * y_box);

    /* returns a box composed of x_box(not modified) and the middle of y_box, needed for midpoint evaluation
     * Inputs: -xy_box: whole box
     *         -y_box: y box to get the middle
     */
    IntervalVector get_mid_y(const IntervalVector& x_box,const IntervalVector& y_box);

    /* set y part of xy_box with y_box
     * */
    IntervalVector init_xy_box(const IntervalVector& x_box,const IntervalVector & y_box);


    IntervalVector xy_box_hull(const IntervalVector& x_box);

    /* add elements of Heap_save into y_heap
     * */
    void fill_y_heap(DoubleHeap<Cell>& y_heap);


	/** Default timeout: 60 */
	static const double default_timeout;


};

} // end namespace ibex
#endif
