//============================================================================
//                                  I B E X
// File        : ibex_LPSolver.h
// Author      : Antoine Marendet
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 15, 2013 (Jordan Ninin)
// Last update : Jan 28, 2020
//============================================================================

#ifndef __IBEX_LP_SOLVER_H__
#define __IBEX_LP_SOLVER_H__

#include <vector>
#include <string>

#include "ibex_CmpOp.h"
#include "ibex_Interval.h"
#include "ibex_Vector.h"
#include "ibex_IntervalVector.h"
#include "ibex_Matrix.h"
#include "ibex_LPLibWrapper.h"
#include "ibex_LPException.h"

namespace ibex {

/**
 * \ingroup numeric
 * \brief Linear Programming Solver
 *
 * Solve a linear problem of the form
 *
 *     Minimizer c^T x              --> objective
*      s.t. l  < x <= u             --> bounds
 *          lhs <= Ax <= rhs        --> constraints
 *
 * The number of variables is fixed at the creation of the LP.
 * However, to avoid reinstantiating a new solver if the number of variables change,
 * which can be heavy depending on the underlying LP library,
 * you can use reset(int) to construct a new LP problem with a different number of variables.
 *
 * The solver provides a infeasibility certification and a objective value enclosure certification
 * using the Neumaier-Shcherbina procedure.
 *
 * Be aware that variables bounds are included as constraints in the LP on the nb_vars first rows.
 * So, for example LPSolver::row(0) will return the bounds constraints for the first variable.
 * The first 'real' constraint will appear only on row nb_vars() (for example, if the problem has 3 variables,
 * you can query the first constraint with row(3)).
 */
class LPSolver {

public:

    /** the default precision on the objective, set to 1e-10. */
	static constexpr double default_tolerance = 1e-9;

    /** Default max_time_out, set to 100s  */
    static constexpr double default_timeout = 100;

    /** Default max_iter, set to 100 iterations */
    static constexpr int default_max_iter = 100;

    /**
     * \brief Status of the linear program solver.
     *
     * It can be either of the followings:
     * - Optimal: the problem has been solved to optimality, with no numerical guarantee,
     * - OptimalProved: the problem has been solved to optimality, and an
     * additional certification step has numerically certified that an optimal value has indeed been found,
     * - Infeasible: the problem is infeasible, with no numerical guarantee,
     * - InfeasibleProved: the problem has been numerically proved to be infeasible,
     * - Unbounded: the problem is unbounded (not proved),
     * - Timeout: abort due to timeout,
     * - MaxIter: abort due to maximum number of iterations reached,
     * - Unknown: the solver is in an unexpected state.
     */
    enum class Status { Optimal, OptimalProved, Infeasible, InfeasibleProved, Unbounded, Timeout, MaxIter, Unknown };

    /**
     * \brief Certification step to do after a solve.
     *
     * This can be either of the followings:
     * - None: no post-processing,
     * - NeumaierShcherbina: if the solver found an optimal solution, compute an
     * interval guaranteed to contain the optimal value of the objective.
     * If the problem is infeasible, try to prove that the problem is infeasible.
     * This procedure is from
     * [Neumaier, A. & Shcherbina, O. Math. Program., Ser. A (2004) 99: 283](https://doi.org/10.1007/s10107-003-0433-3).
     */
    //enum class PostProcessing { None, NeumaierShcherbina };

    enum class Mode { Certified, NotCertified };
    /**
     * \brief Build a LPSolver with nb_vars variables.
     *
     * \param nb_vars number of variables of the linear problem
     * \param mode in certified mode, the result may still be uncertified
     * \param tolerance primal and dual tolerance
     * \param timeout maximum solving time
     * \param max_iter maximum number of iterations
     */
    LPSolver(int nb_vars, LPSolver::Mode mode=LPSolver::Mode::NotCertified, double tolerance=default_tolerance,
        double timeout=default_timeout, int max_iter=default_max_iter);


    /**
     * \brief Read a model file. The accepted format depends on the underlying solver, e.g. for SoPlex .lp and .mps files.
     */
    LPSolver(std::string filename);

    ~LPSolver();

    /**
     * \brief Add a constraint of the type lhs <= row.x <= rhs.
     */
    int add_constraint(double lhs, const Vector& row, double rhs);
    /**
     * \brief Add a constraint of the type row.x op rhs, where op is <= or >=.
     */
    int add_constraint(const Vector& row, CmpOp op, double rhs);

    /**
     * \brief Add multiple constraints.
     */
    void add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs);
    void add_constraints(const Matrix& rows, CmpOp op, const Vector& rhs);

    /**
     * \brief Solve the LP with the current set of constraints,
     * the current objective and bounds.
     * The LP can be modified between two calls to minimize(...)
     * The same instance of the underlying LP solver is updated.
     *
     */
    Status minimize();

    /**
     * \brief Set the objective function vector.
     */
    void set_cost(const Vector& obj);

    /**
     * \brief Set the objective function coefficient for a variable.
     *
     * \param var the variable index
     * \param value the coefficient
     */
    void set_cost(int var, double value);

    /**
     * \brief Set the bounds of the of the variable
     */
    void set_bounds(const IntervalVector& bounds);

    /**
     * \brief Set the bounds on a variable.
     */
    void set_bounds(int var, const Interval& bounds);

    /**
	 * \brief Set both feasibility and optimality tolerance.
	 *
	 * For Cplex, this value must be between 1e-9 and 1e-1. The value is cropped if necessary.
	 * For Soplex, this value must be > 0.
	 */
    void set_tolerance(double tolerance);

    /**
	 * \brief Set the maximum computation time in seconds for a call to minimize().
	 */
    void set_timeout(double timeout);

    /**
	 * \brief Set the maximum number of simplex iterations for a call to minimize().
	 */
    void set_max_iter(int max_iter);

	/**
	 * \brief Number of rows in the LP.
	 * This number includes bounds constraints, so before any call to add_constraint,
	 * this will be equal to nb_vars().
	 */
    int nb_rows() const;

    /**
	 * \brief Number of variables in the LP.
	 */
    int nb_vars() const;

    /**
	 * \brief Feasibility and optimality tolerance.
	 */
    double tolerance() const;

    /**
	 * \brief Maximum number of iterations for a call to minimize().
	 */
    int max_iter() const;

    /**
	 * \brief Maximum computation time for a call to minimize().
	 */
    double timeout() const;

    /**
     * \brief Status of the last call to minimize, or Unknown if minimize has not been called.
     */
    Status status() const;

	/**
	 * \brief Return the constraints matrix, e.g A in lhs <= Ax <= rhs.
	 */
    Matrix rows() const;

	/**
	 * \brief Return a specific row.
	 */
    Vector row(int index) const;

	/**
	 * \brief Return the transposed constraints matrix, A^T.
	 */
    Matrix rows_transposed() const;

	/**
	 * \brief Return a specific column of the constraints matrix.
	 */
    Vector col(int index) const;

	/**
	 * \brief Return the left hand side of the constraints
	 *        (including bounds).
	 */
    Vector lhs() const;

	/**
	 * \brief Return the left hand side of constraint index
	 *        (including bounds).
	 */
    double lhs(int index) const;

    /**
	 * \brief Return the right hand side of the constraints
	 *        (including bounds).
	 */
    Vector rhs() const;

	/**
	 * \brief Return the right hand side of constraint index
	 *        (including bounds).
	 */
    double rhs(int index) const;

    /**
	 * \brief Return the left hand side and right of side of the constraints as an IntervalVector,
	 *        each row corresponding to one constraint (including bounds).
	 */
    IntervalVector lhs_rhs() const;

    /**
	 * \brief Return the left hand side and right hand side of as an Interval for constraint index
	 *        (including bounds).
	 */
    Interval lhs_rhs(int index) const;

    /**
	 * \brief Return the bounds on the variables.
	 */
    IntervalVector bounds() const;

	/**
	 * \brief Return the bounds on variable index.
	 */
    Interval bounds(int index) const;

	/**
	 * \brief Return the cost vector.
	 */
    Vector cost() const;

	/**
	 * \brief Return the cost for variable index.
	 */
    double cost(int index) const;

    /**
     * \brief Wether the last call to minimize() found a feasible point.
     *
     * Return true only if:
     * - in certified mode, status is FeasibleProved
     * - in uncertified mode, status is Feasible
     */
    bool is_feasible() const;

    /**
     * \brief Return an enclosure for the minimum.
     *
     * This is the minimum of the last call to minimize.
     * The enclosure is certified or not depending one the mode defined
     * in the LPSolver constructor.
     * \throws LPException if either minimize() has not been called or the status is not Feasible or FeasibleProved.
     */
    Interval minimum() const;

    /**
     * \brief Return an uncertified primal solution (a minimizer).
     */
    Vector not_proved_primal_sol() const;

    /**
     * \brief Return an uncertified dual solution.
     */
    Vector not_proved_dual_sol() const ;

    // A voir
    /**
     * Return true only if point is proved to be inside the feasible polytope,
     * (even in non certified mode).
     */
    //bool is_feasible_point(const Vector& point) const;

    /**
     * \brief Write the linear problem to a file.
     *
     * The behaviour of this function depends on the lp-lib used.
     * For SoPlex, the format depends on the extension of the file.
     * Supported formats include LP and MPS.
     */
    void write_to_file(const std::string& filename) const;

    // Clear functions
    /**
     * \brief Reset the objective to zero for all variables.
     */
    void set_cost_to_zero();
    /**
     * \brief Remove all constraints, except bound constraints.
     */
    void clear_constraints();

    /**
     * \brief Set all variable bounds to ]-oo, +oo[.
     */
    void clear_bounds();

    /**
     * \brief Reset the solver.
     *
     * Remove constraints, bounds, objective, and change the number of variables
     * to nb_vars.
     */
    void reset(int nb_vars);

private:
    LPSolver::Status status_{LPSolver::Status::Unknown};
    LPSolver::Mode mode_;
    // LP solution is cached
    bool has_solution_ = false;
    Vector uncertified_primal_{1};
    Vector uncertified_dual_{1};

    bool has_infeasible_dir_ = false;
    Vector uncertified_infeasible_dir_{1};
    Interval obj_;

    bool has_changed = true;

    // Bounds on variable in ibex::IntervalVector format.
    IntervalVector ivec_bounds_{1};

    void init(LPSolver::Mode mode, double tolerance, double timeout, int max_iter);
    bool neumaier_shcherbina_postprocessing();
    bool neumaier_shcherbina_infeasibility_test();
    bool uncertified_infeasible_dir(Vector& infeasible_dir) const;

    void invalidate();

    /* This is a macro that should be defined in ibex_LPLibWrapper.h */
	IBEX_LPSOLVER_WRAPPER_ATTRIBUTES;
};

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LPSolver::Status x);

} // end namespace ibex

#endif /* __IBEX_LP_SOLVER_H__ */
