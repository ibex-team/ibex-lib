//============================================================================
//                                  I B E X
// Interface with the linear solver
// File        : ibex_LPSolver.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 15, 2013
// Last Update : May 15, 2013
//============================================================================

#ifndef __IBEX_LP_SOLVER_H__
#define __IBEX_LP_SOLVER_H__

#include "ibex_Setting.h"


#include <string.h>
#include <stdio.h>
#include "ibex_Vector.h"
#include "ibex_Matrix.h"
#include "ibex_IntervalVector.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_Interval.h"
#include "ibex_CmpOp.h"
#include "ibex_Exception.h"
#include "ibex_LPException.h"

@IBEX_LP_LIB_INCLUDES@

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief LP solver.
 *
 * Solve a linear problem
 *
 *     Minimizer c^T x               --> objective
 *     s.t. lhs <= Ax <= rhs         --> constraints
 *           l  < x <= u             --> bounds
 *
 * The same LP solver can be used to solve several problems
 * as long as they all share the same number of variables.
 * The number of constraints may differ and the bounds of variables as well.
 *
 */

class LPSolver {

public:

	/** the default precision on the objective, set to 1e-10. */
	static const double default_eps;

	/** the maximal bound of the variable, set to 1e20. */
	static const double default_max_bound;

    /** Default max_time_out, set to 100s  */
    static const int default_max_time_out;

    /** Default max_iter, set to 100 iterations */
    static const int default_max_iter;


	/** Default max_diam_deriv value, set to 1e6  **/
	static const Interval default_limit_diam_box;

	typedef enum  {OPTIMAL=1, INFEASIBLE=2, OPTIMAL_PROVED=3, INFEASIBLE_PROVED=4, UNKNOWN=0, TIME_OUT=-1, MAX_ITER=-2} Status_Sol;

	typedef enum  {MINIMIZE, MAXIMIZE} Sense;


	/**
	 * \param max_time_out - Control the number of iterations inside the linear solver
	 */
	LPSolver(int nb_vars, int max_iter= default_max_iter,
			int max_time_out= default_max_time_out, double eps=default_eps);

	~LPSolver();

	/**
	 * \brief Solve the LP.
	 *
	 * \return OPTIMAL, INFEASIBLE, UNKNOWN, TIME_OUT or MAX_ITER
	 */
	Status_Sol solve();

	/**
	 * \brief Solve the LP and try to prove the result.
	 *
	 * Call Neumaier-Shcherbina post-processing.
	 *
	 * \note only certify the objective (not the solution point)
	 * \return all possible status, including OPTIMAL_PROVED and INFEASIBLE_PROVED
	 */
	Status_Sol solve_proved();


	void write_file(const char* name="save_LP.lp");


// GET

	/**
	 * \brief Total number of rows
	 *
	 * Rows correspond to all constraints, including bound constraints
	 *
	 */
	int get_nb_rows() const;

	void get_rows(Matrix& A) const;

	void get_rows_trans(Matrix& A_trans) const;

	void get_lhs_rhs(IntervalVector& lhs_rhs) const;

	/**
	 *
	 */
	void get_coef_obj(Vector& obj) const;

	double get_epsilon() const;


	Interval get_obj_value() const;

	void get_primal_sol(Vector & prim) const;

	void get_dual_sol(Vector & dual) const;

	/**
	 * \throw LPExpcetion if not infeasible
	 */
	void get_infeasible_dir(Vector & sol) const;


// SET

	/**
	 * \brief Delete the constraints
	 *
	 * Do not modify the bound constraints
	 * (use clean_bounds or set_bounds)
	 */
	void clean_ctrs();

	/**
	 * \brief Delete the bound constraints
	 *
	 * TODO
	 */
	void clean_bounds() { }

	/**
	 * \brief Set all the objective coefficients to 0.
	 *
	 * TODO
	 */
	void clean_obj() { }

	/**
	 * \brief Clean the LP
	 *
	 * Delete all constraints, including bound constraints
	 * and set the coefficients of the objective to 0.
	 */
	void clean_all();

	void set_max_iter(int max);

	void set_max_time_out(int time);

	void set_sense(Sense s);


	void set_obj(const Vector& coef);

	void set_obj_var(int var, double coef);

	void set_bounds(const IntervalVector& bounds);

	void set_bounds_var(int var, const Interval& bound);

	void set_epsilon(double eps);

	void add_constraint(const Vector & row, CmpOp sign, double rhs );

	void add_constraint(const Matrix & A, CmpOp sign, const Vector& rhs );


private:

	friend class CtcPolytopeHull;

	/**  Call to linear solver to optimize one variable */
	Status_Sol solve_var(Sense sense, int var, Interval & obj);

	/**
	 * Neumaier Shcherbina postprocessing in case of optimal solution found :
	 * the result "obj_value" is made reliable.
	 *
	 * A more efficient variant than NeumaierShcherbina_postprocessing()
	 *
	 * The solution point is *not* made reliable
	 */
	Interval  neumaier_shcherbina_postprocessing_var(int var, Sense sense);


	Interval  neumaier_shcherbina_postprocessing();

	/**
	 *  Neumaier Shcherbina postprocessing in case of infeasibilty found by LP  returns true if the infeasibility is proved
	 */
	bool neumaier_shcherbina_infeasibilitytest();

	/** Definition of the LP */
	int nb_vars;              // number of variables
	int nb_rows;              // total number of rows
	double epsilon;           // precision on the objective
	IntervalVector boundvar;  // bound constraints

	/** =================== Results of the last call to solve() ==================== */
	Interval obj_value;       // (certified or not) enclosure of the minimum
	Vector primal_solution;
	Vector dual_solution;
	int status_prim; // return status of the primal solving (implementation-specific)
	int status_dual; // return status of the dual solving (implementation-specific)
	/**===============================================================================*/

	@IBEX_LP_LIB_EXTRA_ATTRIBUTES@

};

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LPSolver::Status_Sol x);


} // end namespace ibex

#endif /* __IBEX_LP_SOLVER_H__ */
