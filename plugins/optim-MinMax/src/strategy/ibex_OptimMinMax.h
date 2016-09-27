#ifndef __OPTIM_MINIMAX__
#define __OPTIM_MINIMAX__


#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_LightOptimMinMax.h"
#include "ibex_DataMinMax.h"
#include "ibex_Bsc.h"

namespace ibex {

class OptimMiniMax {

public:

    /* Constructor*/
    OptimMiniMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys, Ctc& x_ctc,Ctc& xy_ctc);

    /* Runs a B&B like algorithm
     * arguments: -x_ini: initial x box
     *            -y_ini: initial y box
     *            -prec_x: minimum size of x box
     *            -prec_y: minimum size of y box
     *            -stop_prec: stop criterion
     * */
    void solve(const IntervalVector& x_ini, const IntervalVector& y_ini,double prec_x,double prec_y,double stop_prec);

    double loup;
    double uplo;
    double minprec_uplo;
    IntervalVector best_sol;
    Heap<Cell> x_heap;

    /**
     * \brief destructor
     */
    ~OptimMiniMax();

private:

    Ctc& x_ctc; // contractor w.r.t constraint on x
    NormalizedSystem& x_sys; // contains cst on x and objective function
    LightOptimMinMax lsolve;
    Bsc* bsc;
    CostFuncFmaxlb x_heap_costfunc;
    IntervalVector x_box_init;
    IntervalVector y_box_init;

    double compute_min_prec( const IntervalVector& x_box, double prec_y);
    double choose_nbiter(bool midpoint_eval);
    IntervalVector get_feasible_point(Cell * elem);
    int check_constraints(const IntervalVector& box);
    void handle_cell(Cell * x_cell, double prec_x, double prec_y);
};


}// end namespace

#endif
