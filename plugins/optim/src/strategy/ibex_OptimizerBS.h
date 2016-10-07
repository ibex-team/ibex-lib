//============================================================================
//                                  I B E X
// File        : ibex_OptimizerBS.h
// Author      : Ignacio Araya,  Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Sept 2, 2015
//============================================================================

#ifndef __IBEX_OPTIMIZERBS_H__
#define __IBEX_OPTIMIZERBS_H__

#include "ibex_Bsc.h"

#include "ibex_Backtrackable.h"


#include "ibex_CellSet.h"
#include "ibex_Optimizer.h"


namespace ibex {


/**
 * \ingroup strategy
 *
 * \brief Beam Search Optimizer.
 *
 *
 */
class OptimizerBS : public Optimizer {
public:
	/**
	 *  \brief Create an optimizer.
	 *
	 *   \param sys   - the system to optimize
	 *   \param bsc   - bisector for extended boxes
	 *   \param ctc   - contractor for the <b>extended system</b> (see below)
	 *
	 * And optionally:
	 *   \param prec          - absolute precision for the boxes (bisection control)
	 *   \param goal_rel_prec - relative precision of the objective (the optimizer stops once reached).
	 *   \pram  goal_abs_prec - absolute precision of the objective (the optimizer stops once reached).
	 *   \param sample_size   - number of samples taken when looking for a "loup"
	 *   \param equ_eps       - thickness of equations when relaxed to inequalities
	 *   \param rigor         - look for points that strictly satisfy equalities. By default: false
	 *   \param selnode	   - search strategy (NORMAL, BEAM_SEARCH, BEAM_SEARCH2, PBEAM_SEARCH, DIVING, PROBED_DIVING, DIVING_ITER)
	 *   \param N			   - number of beams for the beam search strategy
	 *   \param prob_bs	   - probability of calling to beam search instead of handle the node normally
	 *   \param M			   - number of treated leaves before the beam search is called again
	 *   \param initset   	   - if true, the beam search begins from the N best boxes in bufferset, otherwise it begins with the best box
	 *   \param alpha		   - boxes such that lb(xo) > (1-alpha)*uplo + alpha*loup are discarded from the beam search
	 *
	 * <ul> The extended system (see ExtendedSystem constructor) contains:
	 * <li> (n+1) variables, x_1,...x_n,y. The index of y is #goal_var (==n).
	 * <li> A (m+1)-valued function f:(x,y)->(y-f(x),g_0(x),...,g_m(x))
	 * <li> (m+1) constraints: y-f(x)=0, g_1(x)<=0, ..., g_m(x)<=0.
	 * </ul>
	 *
	 * \warning The optimizer relies on the contractor \a ctc to contract the domain of the goal variable and increase the uplo.
	 * If this contractor never contracts this goal variable, the optimizer will only rely on the evaluation of f  and will be very slow.
	 *
	 */

	OptimizerBS(System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& bufferset, double prec=default_prec,
			double goal_rel_prec=default_goal_rel_prec, double goal_abs_prec=default_goal_abs_prec,
			  int sample_size=default_sample_size, double equ_eps=default_equ_eps, bool rigor=false, int selnode=NORMAL, double N=1.0,  double prob_bs=1.1, bool initset=false);

	/**
	 * \brief Delete *this.
	 */
	virtual ~OptimizerBS();

	/**
	 * \brief Run the optimization.

	 * \param init_box       -  the initial box
	 * \param obj_init_bound - (optional) can be set when an initial upper
	 *                         bound of the objective minimum is known a priori.
	 *                         (this bound can be obtained, e.g., by a local solver).
	 */
    virtual Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);



	enum strategy{NORMAL, BEAM_SEARCH, IBEST_FIRST_MINLB, IBEST_FIRST_MAXDEPTH,IBEST_FIRST_DEEPFIRST};
	double factor_adaptive;

 protected:
    //~ std::set<OptimCell *,minLB> buffer2;

	/**
	 * \brief Main procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract and bound the cell box (see contract_and_bound)
	 * <li> return true iff the cell has not been deleted
	 * </ul>
	 *
	 */
        void update_cell_data(Cell& c);

	void handle_node(Cell* c, CellBuffer& L, const IntervalVector& init_box );
	void beam_search(CellBuffer& L, const IntervalVector& init_box);
	void ibest_first(CellBuffer& L, const IntervalVector& init_box, int selnode);
	//~ void probed_diving(CellBuffer& L, const IntervalVector& init_box);
	//~ void diving_iter(CellBuffer& L, const IntervalVector& init_box);
	//~ void diving(CellBuffer& L, const IntervalVector& init_box);


	double getminimum() const;



	 /* KBFS *************/
	//~ double box_loup;
	//~ Vector box_point;

	int selnode;
	
	double N;
	double prob_bs;
	bool initset;



   
    /* *******************/

};


} // end namespace ibex
#endif // __IBEX_OPTIMIZERBS_H__
