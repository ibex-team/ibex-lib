#ifndef __OPTIM_MINIMAX__
#define __OPTIM_MINIMAX__


#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_LightOptimMinMax.h"
#include "ibex_DataMinMax.h"
#include "ibex_Bsc.h"

namespace ibex {

class OptimMiniMax : Optim {

public:

    /* Constructor*/
    OptimMiniMax(NormalizedSystem& x_sys, NormalizedSystem& xy_sys, Ctc& x_ctc,Ctc& xy_ctc,double prec_x,double prec_y,double stop_prec);

    /* Runs a B&B like algorithm
     * arguments: -x_ini: initial x box
     *            -y_ini: initial y box
     *            -prec_x: minimum size of x box
     *            -prec_y: minimum size of y box
     *            -stop_prec: stop criterion
     * */

	/**
	 * \brief Run the optimization.
	 *
	 * \param init_box             The initial box
	 * \param obj_init_bound       (optional) can be set when an initial upper bound of the objective minimum is known a priori.
	 *                             This bound can be obtained, e.g., by a local solver. This is equivalent to (but more practical
	 *                             than) adding a constraint f(x)<=obj_init_bound.
	 *
	 * \return SUCCESS             If the global minimum (with respect to the precision required) has been found.
	 *                             In particular, at least one feasible point has been found, less than obj_init_bound, and in the time limit.
	 *
	 *         INFEASIBLE          if no feasible point exist less than obj_init_bound. In particular, the function returns INFEASIBLE
	 *                             if the initial bound "obj_init_bound" is LESS than the true minimum (this case is only possible if
	 *                             goal_abs_prec and goal_rel_prec are 0). In the latter case, there may exist feasible points.
	 *
	 *         NO_FEASIBLE_FOUND   if no feasible point could be found less than obj_init_bound. Contrary to INFEASIBLE,
	 *                             infeasibility is not proven here. Warning: this return value is sensitive to the goal_abs_prec and
	 *                             goal_rel_prec parameters. The upperbounding makes the optimizer only looking for points less than
	 *                             min { (1-goal_rel_prec)*obj_init_bound, obj_init_bound - goal_abs_prec }.
	 *
	 *         UNBOUNDED_OBJ       the objective function seems unbounded (tends to -oo).
	 *
	 *         TIMEOUT             time is out.
	 */
	virtual Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);


    /**
     * \brief destructor
     */
    ~OptimMiniMax();

    IntervalVector x_box_init;
    IntervalVector y_box_init;
//TODO
    void set_prec_y(double prec_y);

private:

    Ctc& x_ctc; // contractor w.r.t constraint on x
    NormalizedSystem& x_sys; // contains cst on x and objective function
    LightOptimMinMax lsolve;
    Bsc* bsc;

    double compute_min_prec( const IntervalVector& x_box, double prec_y);
    int choose_nbiter(bool midpoint_eval);
    IntervalVector get_feasible_point(Cell * elem);
    int check_constraints(const IntervalVector& box);
    void handle_cell(Cell * x_cell, double prec_x, double prec_y);
};


}// end namespace

#endif
