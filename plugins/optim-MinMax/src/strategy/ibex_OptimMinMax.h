//============================================================================
//                                  I B E X
// File        : ibex_OptimMinMax.h
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================


#ifndef __OPTIM_MINIMAX__
#define __OPTIM_MINIMAX__


#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_LightOptimMinMax.h"
#include "ibex_Bsc.h"
#include "ibex_Optim.h"
#include "ibex_LightLocalSolver.h"

namespace ibex {

class OptimMinMax : public Optim {

public:

    /* Constructor*/
    OptimMinMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys, Ctc& x_ctc,Ctc& xy_ctc,double prec_x,double prec_y,double goal_rel_prec);

//    OptimMinMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys, Ctc& x_ctc,Ctc& xy_ctc,double prec_x,double prec_y,double goal_rel_prec,Function& max_goal);

    /* Constructor, with "for all y" constraints
     * for all constraint in the objectif function of max_fa_y_cst lower than 0. If several for all constraints the objectif is equal to the max
     * of constraint functions lower than 0
     */

    OptimMinMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys,NormalizedSystem& max_fa_y_cst, Ctc& x_ctc,Ctc& xy_ctc,
                             double prec_x,double prec_y,double goal_rel_prec,double fa_cst_prec);

//    OptimMinMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys,NormalizedSystem& max_fa_y_cst, Ctc& x_ctc,Ctc& xy_ctc,
//                             double prec_x,double prec_y,double goal_rel_prec,double fa_cst_prec,Function& max_goal,Function& max_goal_fa);

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
        Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);
	Status optimize();


    /**
     * \brief destructor
     */
    virtual ~OptimMinMax();

    IntervalVector x_box_init;
    IntervalVector y_box_init;
    IntervalVector y_box_init_fa; // y box for constraints of type for all y may be different than y_box_ini

    inline void set_prec_y(double prec_y) {this->prec_y = prec_y; }

    int trace_freq;

    //light solver param for objectif function
    int list_rate; // rate of growth of y_heap size, see compute_heap_max_size function for formula detail
    int list_elem_absolute_max;
    int iter; // number of iteration of lightsolver allowedNormalizedSystem *
    double min_perc_coef; // used to compute y_prec allowed when run the light solver, see compute_min_prec function for formula
    int critpr; // probability to choose second heap in light_solver
    int local_iter; // number of local solver run

    //light solver param for csp
    int list_rate_csp; // rate of growth of y_heap size, see compute_heap_max_size function for formula detail
    int list_elem_absolute_max_csp;
    int iter_csp; // number of iteration of lightsolver allowedNormalizedSystem *
    double min_perc_coef_csp; // used to compute y_prec allowed when run the light solver, see compute_min_prec function for formula
    int critpr_csp; // probability to choose second heap in light_solver
    int local_iter_csp; // number of local solver run
    bool only_csp; // solve a csp problem, no objective function.

    // light local solver param
    double nb_sols;
    double min_acpt_diam;
    int nb_sivia_iter;
    int nb_optim_iter;
    double y_sol_radius;
    double reg_acpt_error;


    bool monitor; // create log file if true
    bool monitor_csp; // create a "paver.txt" file that contains boxes plus a value which indicates whether constraints are respected or not
    // each line of paver.txt corresponds to: x[0].lb x[0].ub x[1].lb ... x[n].ub val, val=0 -> constraint not respected, val=1 -> maybe, val=2 -> constraint respected

    int heap_prob;// probability between 0 and 100 to choose heap 2 over 1 in the main B&B, heap 1 is sorted from the lower lb to the greatest, heap 2 from the greatest ub to the lowest

    bool visit_all;// if true force to visit all leaves of slave heap in max optim
    bool visit_all_csp;// if true force to visit all leaves of slave heap in max optim of csp solver

    int nb_point; // number of point evaluation in current box x (use to compute uplo)
    double perf_thresh; // cell is spawn from loc_solve csp list if its perf_csp is lower than perf_thresh

private:

    Ctc& x_ctc; // contractor w.r.t constraint on x
    NormalizedSystem& x_sys; // contains cst on x and objective function
    LightOptimMinMax lsolve;
    LightLocalSolver loc_solve;
    Bsc* bsc;
    double prec_y;
    bool fa_y_cst; // indicates if there is for all y constraints
    Function* minus_goal_y_at_x; // goal function f becomes -f to solve a minimization problem over y at a fixed x
    UnconstrainedLocalSearch * local_search;


    void init_lsolve();
    void init_fa_lsolve();
    void init_loc_solve();
    double compute_min_prec( const IntervalVector& x_box,bool csp);
    int choose_nbiter(bool midpoint_eval,bool csp);
    int compute_heap_max_size(int y_heap_size,bool csp);
    bool get_feasible_point(Cell * elem);
    int check_constraints(Cell * x_cell,bool midp);
    int check_regular_ctr(const IntervalVector& box);
    int check_fa_ctr(Cell * x_cell,bool midp);
    bool handle_cell(Cell * x_cell);
    bool spawn(Cell* x_cell); // spawn a cell into subcells from the csp list of loc_solve if perf crit is lower than spawn_thresh


    // Fa cst variables
    double prec_fa_y;
    LightOptimMinMax fa_lsolve;
    Function* minus_goal_csp_y_at_x;// goal csp function g becomes -g to solve a minimization problem over y at a fixed x
    UnconstrainedLocalSearch * local_search_csp;
    LightLocalSolver fa_loc_solve;



    //Default parameters for light optim min max solver
    static const int default_list_rate;
    static const int default_list_elem_absolute_max;
    static const int default_iter;
    static const double default_min_perc_coef;
    static const int default_prob_heap;
    static const int default_local_iter;
    static const bool default_visit_all;
    static const int default_nb_point;
    static const double default_perf_thresh;

    //Default parameters for light local solver

    static const double default_nb_sols;
    static const double default_min_acpt_diam;
    static const int default_nb_sivia_iter;
    static const int default_nb_optim_iter;
    static const double default_y_sol_radius;
    static const double default_reg_acpt_error;

    //Csp fa function default parameters for light solver
    static const int default_list_rate_csp;
    static const int default_list_elem_absolute_max_csp;
    static const int default_iter_csp;
    static const double default_min_perc_coef_csp;
    static const int default_prob_heap_csp;
    static const int default_local_iter_csp;
    static const bool default_visit_all_csp;


};
void export_monitor(std::vector<double> * ub,std::vector<double> * lb,std::vector<double> * nbxel,std::vector<double> * nbyel);

}// end namespace

#endif
