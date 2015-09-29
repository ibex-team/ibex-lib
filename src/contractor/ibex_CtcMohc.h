//============================================================================
//                                  I B E X
// File        : Mohc Contractor
// Author      : Ignacio Araya, Gilles Trombettoni, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Ene 8, 2013
// Last Update : Ene 8, 2013
//============================================================================


#ifndef __IBEX_CTC_MOHC_H__
#define __IBEX_CTC_MOHC_H__

#include "ibex_CtcPropag.h"
#include "ibex_ExprSplitOcc.h"
#include "ibex_NumConstraint.h"
#include "ibex_HC4Revise.h"

#include <vector>
#include <list>

namespace ibex {

class CtcMohcRevise;

/**
 * \brief Occurrence Grouping algorithm.
 *
 * This class is an implementation of the <i>Occurrence Grouping</i> algorithm used mainly by
 * the Mohc algorithm (see \link CtcMohc \endlink).
 * See description in <a href="http://www-sop.inria.fr/coprin/trombe/publis/og_computing_2011.pdf">[ara12]</a>.
 *
 * The following code shows how to perform the evaluation of a function f in a box.
 * ...
 * Function_OG fog(f);
 * fog.occurrence_grouping(box); //calling only once to this function, every evaluation we perform inside the box is valid
 * fog.eval(box);
 * ...
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */
class Function_OG {
	friend class CtcMohcRevise;

public:

	/**
	 * \brief Function_OG constructor
	 *
	 * Creates an object Function_OG which takes the function f and constructs a new function fog in which each occurrence j of a variable x in f has been replaced by a sum r_a[j]*xa + r_b[j]*xb + r_c[j]*xc
	 * \param f The original function
	 */
	Function_OG(const Function& f);

	/**
	 * \brief The Occurrence Grouping algorithm
	 *
	 * See description in <a href="http://www-sop.inria.fr/coprin/trombe/publis/og_computing_2011.pdf">[ara12]</a>.
	 * Performs the Occurrence Grouping algorithm.
	 * \param box The box
	 * \param y_set if it is true it indicates that the variables occuring once also should be grouped (by default it is false)
	 * \param _og <i>only for analysis purposes</i>
	 */
	bool occurrence_grouping(IntervalVector& box, bool y_set=false, bool _og=true);

	/** performs an evaluation of the function using the grouping previously performed by #occurrence_grouping(IntervalVector& , bool, bool)**/
	Interval eval(IntervalVector& box);

	/** performs an evaluation of the lower/upper bound of the function using the grouping previously performed by #occurrence_grouping(IntervalVector& , bool, bool)
	 * \param box The box
	 * \param minrevise if it is true, it performs an evaluation of the lower bound of the box, otherwise, an evaluation of the upper bound is performed
	 */
	Interval eval(IntervalVector& box, bool minrevise);

	/** performs a call to the MinRevise/MaxRevise algorithm using fog (see \link CtcHC4Revise \endlink, \link CtcMohc \endlink, <a href="http://www-sop.inria.fr/coprin/trombe/publis/mohc_aaai_2010.pdf">[ara10]</a>)**/
	Interval revise(IntervalVector& box, bool minrevise);


private:

	void set_ra(int i, Interval val);
	void set_rb(int i, Interval val);
	void set_rc(int i, Interval val);

	bool gradient(IntervalVector& box);

	bool occurrence_grouping(int i, bool _og);

	void _eval_leaves(IntervalVector& box, bool minrevise);
	void _proj_leaves(IntervalVector& box);

	//initialize the occurrence-based box using a normal box variable-based
	void _setbox(IntervalVector& box);

	void OG_case1(int i);
	void OG_case2(int i, Interval inf_G_Xa, Interval inf_G_Xb, Interval sup_G_Xa, Interval sup_G_Xb);
	void OG_case3(std::list<int>& X_m, std::list<int>& X_nm, Interval& G_m);

	ExprSplitOcc eso;
	Function _f;

	IntervalVector r_a;
	IntervalVector r_b;
	IntervalVector r_c;

	/* maps from var i, occ j -> to var in fog.
	 * furthermore occ[i].size() is the number of occurrences of var i */
	std::vector<int>* occ;

	IntervalVector _box;

	/* partial derivatives w.r.t. each occurrence */
	IntervalVector _g;

	/*partial derivatives w.r.t. the original variables*/
	IntervalVector g;

	/*partial derivatives w.r.t. x_a*/
	IntervalVector ga;

	/*partial derivatives w.r.t. x_b*/
	IntervalVector gb;

	/*used for performing minrevise (resp. maxrevise),
	 * it saves the constant: r_a * lb(xk) + r_b * ub(xk) for each occurrence xk */
	IntervalVector aux;

	/*** methods only used by \link CtcMohcRevise \endlink***/
	Interval Newton_it(Interval b, double x_m, double f_m, int i);
	Interval Newton_it(Interval b, double x_m, double f_m, int i, bool increasing);
	Interval Newton_it_cert(Interval b, double x_m, double f_m, int i);
	void MonoUnivNewton(IntervalVector& box, int i, Interval& b, bool minEval, double w, double min_width);
	/**********************************************************/

};

/** \ingroup contractor
 * \brief MohcRevise algorithm.
 *
 * This class is an implementation of the <i>Mohc-Revise</i> algorithm.
 * See description in <a href="http://www-sop.inria.fr/coprin/trombe/publis/mohc_aaai_2010.pdf">[ara10]</a>.
 *
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */
class CtcMohcRevise : public Ctc {

public:
	/**
	 * \brief Mohc-Revise constructor
	 *
	 * Creates the Mohc-Revise contractor for handling/revising a single constraint.
	 * \param ctr The constraint
	 * \param epsilon Precision ratio (percentage of interval width) to rather set between 0.01 and 0.1. See class \link CtcMohc \endlink.
	 * \param univ_newton_min_width Minimum width of a slice in the <i>MonotonicBoxNarrow</i> procedure. The default value is 1e-8 but the actual default value should depend on the precision of the solutions.
	 * \param tau_mohc Ratio to rather set between 0.5 and 0.9999. See class \link CtcMohc \endlink.
	 * \param amohc If it is true, the value of tau_mohc is tuned automatically (the value given to \a tau_mohc is not taken into account). See class \link CtcMohc \endlink.
	 */
	CtcMohcRevise(const NumConstraint& ctr, double epsilon, double univ_newton_min_width,
			double tau_mohc, bool amohc);

	/** Contract the box using <i>Mohc-Revise</i> in the constraint \a ctr_mohc. */
	virtual void contract(IntervalVector& box);

	/** The constraint. */
	const NumConstraint ctr;

	/** The og function. */
	Function_OG fog;

	/** Perform \a LazyNarrow*/
	void LazyNarrow();

	/** Perform \a MonotonicBoxNarrow*/
	void MonoBoxNarrow();

	/**
	 * Perform \a MonotonicBoxNarrow to the i-th variable of \a ctr_mohc
	 * \param x The interval been contracted
	 */
	void MonoBoxNarrow(int i);

	/** The value related with the activation of the monotonic procedures.
	 * If active_mono_proc is 1, we should perform the monotonic procedures.
	 * If active_mono_proc is 0, we should not perform them.
	 * active_mono_proc = -1 indicates that the activation should be initialized (by the private method update_active_mono_proc(..)). */
	int active_mono_proc;


	//   static double tau_mono;

	/* activation of MohcRevise's components. (For analysis purposes only)*/
	static bool _minmax;
	static bool _monobox;
	static bool _opt;
	static bool _og;
	static bool _mohc2;
	/**********************************************************************/



private:

	/* For contracting partially monotonic variables (generated by OG) */
	void LeftNarrow(int i);
	void RightNarrow(int i);
	void initLeftNarrow(int i);
	void initRightNarrow(int i);
	/*************/

	void initLeftNarrowFmin(int i);
	void initLeftNarrowFmax(int i);
	void initRightNarrowFmin(int i);
	void initRightNarrowFmax(int i);

	void LeftNarrowFmin(int i);
	void LeftNarrowFmax(int i);
	void RightNarrowFmin(int i);
	void RightNarrowFmax(int i);

	void initialize_apply();
	void apply_fmax_to_false_except(int i);
	void apply_fmin_to_false_except(int i);

	bool hasMultOcc(Function &f);
	bool _existence_test(int i);

	void update_active_mono_proc(Interval& z);

	enum _3vl {MAYBE,YES,NO};
	_3vl* ApplyFmin;
	_3vl* ApplyFmax;

	//Arrays used for saving the lower and upper bounds of the variables
	//in the monotonicboxnarrow procedure. At the end of the procedure x[i] <- Interval( Inf(LB[i]), Sup(RB[i]) )
	IntervalVector LB;
	IntervalVector RB;

	Interval zmin,zmax;
	IntervalVector box;

	double tau_mohc;
	double epsilon;
	double univ_newton_min_width;
	bool amohc;


};

/** \ingroup ctcgroup
 * \brief Mohc algorithm
 *
 * This class is an implementation of the <i>Mohc</i> algorithm described in <a href="http://www-sop.inria.fr/coprin/trombe/publis/mohc_aaai_2010.pdf">[ara10]</a>.
 * In order to use <i>Mohc</i> inside a shaving contractor, it is necessary to perform a preprocessing in the current box. The preprocessing updates the array mohc->active_mono_proc which is used by <i>Mohc</i> inside the shaving contractor.
 * The following code shows how we can generate a shaving contractor which uses <i>Mohc</i> (e.g., \link Ctc3BCid \endlink):
 * \code
 * #include "ibex_CtcMohc.h"
 * #include "ibex_CtcCompo.h"
 * #include "ibex_Ctc3BCid.h"
 * ...
 * CtcMohc mohc(csp); //performs the preprocessing and first call to mohc in the current box
 * CtcMohc mohc_in_shav(csp, mohc->active_mono_proc); //the mohc algorithm which is used inside the shaving
 * CtcCompo shav_mohc(mohc,Ctc3BCid(mohc_in_shav));
 * ...
 * \endcode
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */

class CtcMohc : public CtcPropag {
public:

	/** Creates the <i>Mohc</i> contractor.
	 * \param csp The set of constraints.
	 * \param ratio The precision ratio (percentage of interval width) in the propagation loop. See \a CtcPropag.
	 * \param incremental A boolean indicating whether the propagation is incrementally run from the last variable that has been bisected in the search tree. To be generally set to true when \a Mohc is the only contractor called between two bisections. See \a IbexPropagation.
	 * \param epsilon The precision ratio (percentage of interval width) of the shaving process performed by the monotonic BoxNarrow procedure (see the method \a MonoBoxNarrow). Remark: when called inside a solving strategy (i.e., a tree search), <i>Mohc</i> is not very sensitive to this parameter, contrarily to the standard <i>Box</i> contractor. Experiments on zero-dimensional n*n benchmarks (see Araya et al. in Proc. CP 2010 and details in the PhD thesis of Ignacio Araya) suggest that epsilon=3% should be relevant for any system when <i>Mohc</i> is called "alone" between two choice points in the search tree, whereas epsilon=10% should be "always" adequate (and sufficient) when <i>Mohc</i> is a sub-contractor of a shaving contractor, such as <i>3B</i> or <i>3BCID</i> (see \link Ctc3BCid \endlink).
	 * \param univ_newton_min_width Minimum width of a slice in the <i>MonotonicBoxNarrow</i> procedure. The default value is 1e-8 but the actual default value should depend on the precision of the solutions
	 * \param tau_mohc tau_mohc Ratio to rather set between 0.5 and 0.9999. \a tau_mohc is the most significant user-defined parameter of \a Mohc. If \c tau_mohc=1, this means that the <i>MohcRevise</i> procedure will always call the "monotonic" contractors, namely <i>MinRevise</i>, <i>Maxrevise</i> and <i>MonoBoxNarrow</i>; If \c tau_mohc=0.6, this means that, for a given constraint ctr, <i>MohcRevise</i> will call the monotonic contractors only if rho_mohc(ctr) is inferior to 0.6, i.e., if the width of the interval image of ctr evaluated by monotonicity is less than 60% of the image width obtained by natural evaluation.
	 * If \c tau_mohc=-1, then the parameter is computed by using an adaptive procedure (amohc). This procedure will set
	 * \c tau_mohc to 0.5 or 0.99 depending on how often the evaluation by using the monotonicity is "enough better"
	 * than the natural evaluation.
	 * Remark 1: In practice, when Mohc is used as a subcontractor of \a Ctc3BCid, the default value (0.6 or 0.5) would ensure that <i>Mohc</i> show performances either significantly better than HC4 (see \link CtcHC4 \endlink) or similar, but hopefully never worse. Please contact the authors if it occurs.
	 */


	CtcMohc(const Array<NumConstraint>& csp, double ratio=default_ratio, bool incremental=false,  double epsilon=default_epsilon,
			double univ_newton_min_width=default_univ_newton_min_width, double tau_mohc=ADAPTIVE);


	/** Creates the <i>Mohc</i> contractor. This constructor allows to specify the array \param active_mono_proc.
	 * This array indicates which constraints should call the "monotonic procedures" of \a MohcRevise. If \param active_mono_proc
	 * is not specified, then all the constraints (having multiple occurrences of variables) are considered by the monotonic procedures.
	 * \param active_mono_proc may be generated by the incremental variant of \a Mohc.
	 *
	 * Example of use:
	 * #include "ibex_CtcMohc.h"
	 * #include "ibex_CtcCompo.h"
	 * #include "ibex_Ctc3BCid.h"
	 * ...
	 * CtcMohc mohc(csp); //performs the preprocessing and first call to mohc in the current box
	 * CtcMohc mohc_in_shav(csp, mohc->active_mono_proc); //the mohc algorithm which is used inside the shaving
	 * CtcCompo shav_mohc(mohc,Ctc3BCid(mohc_in_shav));
	 * ...
	 *
	 * In this example, we have created a contractor that calls Mohc and then 3BCid(Mohc) (see \a Ctc3BCid). Each time the contractor is called,
	 * the first instance of Mohc (mohc) will fill the active_mono_proc array that will be used then by the second
	 * instance of mohc (mohc_in_shav).
	 * \param csp The set of constraints.
	 * \param active_mono_proc The array that Mohc will use (and will not update) to decide if the monotonic based procedures will be applied in MochRevise:
	 * If active_mono_proc[f] is 1, MohcRevise performs the monotonic procedures on f.
	 * If active_mono_proc[f] is 0, MohcRevise does not perform them.
	 * These arrays should be initialized in other instance of Mohc previously called.
	 * \param ratio The precision ratio (percentage of interval width) in the propagation loop. See \a CtcPropag.
	 * \param incremental A boolean indicating whether the propagation is incrementally run from the last variable that has been bisected in the search tree. To be generally set to true when \a CtcMohc is the only contractor called between two bisections. See \a CtcPropag.
	 * \param epsilon The precision ratio (percentage of interval width) of the shaving process performed by the monotonic BoxNarrow procedure (see the method \a MonoBoxNarrow). Remark: when called inside a solving strategy (i.e., a tree search), <i>Mohc</i> is not very sensitive to this parameter, contrarily to the standard <i>Box</i> contractor. Experiments on zero-dimensional n*n benchmarks (see Araya et al. in Proc. CP 2010 and details in the PhD thesis of Ignacio Araya) suggest that epsilon=3% should be relevant for any system when <i>Mohc</i> is called "alone" between two choice points in the search tree, whereas epsilon=10% should be "always" adequate (and sufficient) when <i>Mohc</i> is a sub-contractor of a shaving contractor, such as <i>3B</i> or <i>3BCID</i> (see \link Ctc3BCid \endlink).
	 * \param univ_newton_min_width Minimum width of a slice in the <i>MonotonicBoxNarrow</i> procedure. The default value is 1e-8 but the actual default value should depend on the precision of the solutions
	 */

	CtcMohc(const Array<NumConstraint>& csp, int* active_mono_proc, double ratio=default_ratio, bool incremental=false,  double epsilon=default_epsilon,
			double univ_newton_min_width=default_univ_newton_min_width);

	~CtcMohc();

	static const double ADAPTIVE;

	/** Contract the box using <i>Mohc</i> in the set of constraints \a csp. */
	virtual void contract(IntervalVector& box){

		//initialization of the value active_mono_proc for each constraint
		//if the first contractor was used, the values are set to -1
		//(they are computed by the method CtcMohcRevise::update_active_mono_proc)
		//otherwise, they are copied from the array active_mono_proc
		for(int i=0;i<list.size();i++){
			CtcMohcRevise* ctc= dynamic_cast<CtcMohcRevise*>(&list[i]);
			ctc->active_mono_proc=(update_active_mono_proc)? -1:active_mono_proc[i];
		}

		CtcPropag::contract(box);

		//the array active_mono_proc is updated
		for(int i=0;i<list.size();i++){
			CtcMohcRevise* ctc= dynamic_cast<CtcMohcRevise*>(&list[i]);
			active_mono_proc[i]=ctc->active_mono_proc;
		}

	}

	/** Default \a tau_mohc value, set to 0.5  **/
	static const double default_tau_mohc;

	/** Default \a epsilon value, set to 0.1  **/
	static const double default_epsilon;

	/** Default \a min_width value, set to 1e-8 **/
	static const double default_univ_newton_min_width;

	/*********** amohc ******/
	//parameters
	static const double RHO_INTERESTING;
	static const double TAU_FREQ;

	//auxiliary variables
	static int nb_calls;
	static int nb_interesting;
	/*************************/


	/** The array related with the activation of the monotonic procedures in MohcRevise.
	 * If active_mono_proc[f] is 1, we should perform the monotonic procedures of f.
	 * If active_mono_proc[f] is 0, we should not perform them.
	 * active_mono_proc[f] = -1 indicates that the activation has not be initialized. */
	int* active_mono_proc;




private:

	/** If it is true, then, in each propagation, the first time a constraint [f]
	 * is revised, its related value active_mono_proc[f] is updated.*/
	bool update_active_mono_proc;

};
}
#endif
