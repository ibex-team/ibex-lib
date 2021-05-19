/* ============================================================================
 * I B E X - Iterative loup finder
 * ============================================================================
 *
 * Author(s)   : Ignacio Araya, Victor Reyes
 * Created     : May 2021
 * Updated     : May 2021
 * ---------------------------------------------------------------------------- */


#ifndef __IBEX_LOUP_FINDER_ITERATIVE_H__
#define __IBEX_LOUP_FINDER_ITERATIVE_H__

#include "ibex_LoupFinder.h"
#include "ibex_System.h"
#include "ibex_LoupFinderIP.h"
#include "ibex_Vector.h"

namespace ibex {

class LoupFinderIterative : public LoupFinder {

	/**
	 * \ingroup optim
	 *
	 * \brief An iterative algorithm based on XTaylor and/or AbsTaylor.
	 *
	 * The algorithm builds an inner (feasible) polytope inside the
	 * current box by using either AbsTaylor or XTaylor; and then minimizes a
	 * linear approximation of the goal function on this polytope via
	 * a LP solver. If the algorithm success, then it construct a new box (inside the
	 * search space), continuing the search of better upperbounds.
	 *
	 * For more details check the paper: Reyes, V., & Araya, I. (2021). AbsTaylor: upper
	 * bounding with inner regions in nonlinear continuous global optimization problems.
	 */

public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys   - The NLP problem.
	 * \param initial_box - The initial search space.
	 * \param loup_finder - List of loup finders. The complete list is used to find a first loup. Then
	 * 						the first element of the list is used during the iteration process.
	 * \param alpha  - parameter to reduce the box
	 * \param max_iter - maximum number of iterations
	 * \param prec - the precision of the algorithm
	 */

	LoupFinderIterative(const System& sys, const IntervalVector& initial_box,std::vector<LoupFinderIP*> loup_finders=std::vector<LoupFinderIP*>(0), double alpha=0.9,int max_iter=10, double prec=1e-3);

	/**
	 * \brief Delete this.
	 */
	virtual ~LoupFinderIterative();

	/**
	 * \brief Find a new loup in a given box and the neighborhood by using
	 * a LoupFinder (e.g. AbsTaylor/XTaylor).
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);
	/**
	 * \brief To use for checking the progress of the upperbounds
	 */
	void set_trace(bool trace){this->trace = trace;}
	/**
	 * \brief For printing the ub, in case the trace function is active
	 */
	void print_ub(std::pair<IntervalVector,double> p);
	/**
	 * \brief Changes the size and position of the current search box. Always inside
	 * the root box.
	 */
	void reduce_and_move(IntervalVector& box_aux, Vector old_exp);
	/*
	 * \brief This function constructs the list of the "default" loop finders.
	 */
	void get_default_finders(std::vector<LoupFinderIP*> & loup_finders);
	/**The system**/
	const System& sys;
private:

	/**LoupFinder IP - AbsTaylor/XTaylor**/
	std::vector<LoupFinderIP*> loup_finders;
	/**The initial box (search space)**/
	const IntervalVector& initial_box;
	/**User parameter for convergence purposes**/
	double alpha;
	/*maximum number of iterations*/
	int max_iter;
	/*the precision */
	double prec;
	/*trace, just for testing & example purposes*/
	bool trace;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_ITERATIVE_H__ */
