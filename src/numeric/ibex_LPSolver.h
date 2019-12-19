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
 *     Minimizer c^T x               --> objective
 *     s.t. lhs <= Ax <= rhs         --> constraints
 *           l  < x <= u             --> bounds
 *
 * The number of variables is fixed at the creation of the LP.
 * However, to avoid reinstantiating a new solver if the number of variables change,
 * which can be heavy depending on the underlying LP library,
 * you can use reset_with_new_vars(int) to construct a new LP problem with a different number of variables.
 *
 * The solver provides a infeasibility certification and a objective value enclosure certification
 * using the Neumaier-Shcherbina procedure.
 */
class LPSolver {

public:

    /** the default precision on the objective, set to 1e-10. */
	static constexpr double default_tolerance = 1e-9;

    /** Default max_time_out, set to 100s  */
    static constexpr double default_timeout = 100;

    /** Default max_iter, set to 100 iterations */
    static constexpr int default_max_iter = 100;

    /** Default minimal diameter of ??, set to 1e-14  **/
	static constexpr double min_box_diam = 1e-14;

    /** the maximal bound of the variable, set to 1e20. */
	static constexpr double default_max_bound = 1e20;

	/**
	 * TODO: Default maximal diameter of ... what exactly?
	 *   In LoupFinderXTaylor: -> the box
	 *   In LinearizerXTaylor : -> the derivative enclosure
	 * Set to 1e6.
	 */
	static constexpr double max_box_diam = 1e6;

    /**
     * \brief Status of the linear program solver.
     *
     * It can be either of the followings:
     * - Optimal: the problem has been solved to optimality, with no numerical guarantee,
     * - OptimalProved: the problem has been solved to optimality, and an
     * additional certification step has numerically certified that an optimal value has indeed been found,
     * - Infeasible: the problem is infeasible,
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
    enum class PostProcessing { None, NeumaierShcherbina };

    /**
     * \brief Build a LPSolver with nb_vars variables.
     *
     * \param nb_vars number of variables of the linear problem
     * \param tolerance primal and dual tolerance
     * \param timeout maximum solving time
     * \param max_iter maximum number of iterations
     */
     // TODO Determiner à la création si certifié ou pas
    LPSolver(int nb_vars=0, double tolerance=default_tolerance,
        double timeout=default_timeout, int max_iter=default_max_iter);


    /**
     * \brief Read a model file
     */
    LPSolver(std::string filename);

    ~LPSolver();

    /*
    void add_variable(const Interval& bounds, double obj);
    void add_variable(const Vector& col, const Interval& bounds, double obj);
    void add_variables(const IntervalVector& bounds, const Vector& obj);
    void add_variables(const Matrix& cols, const IntervalVector& bounds, const Vector& obj);
    */

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

    // minimize()
    /*
     * Solve the LP with the current set of constraints,
     * the current objective and bounds.
     * The LP can be modified between two calls to optimze(...)
     * The same instance of the underlying LP solver is updated.
     *
     */
    Status optimize(PostProcessing pp = PostProcessing::None);
    Status optimize_proved();

    /**
     * \brief Set the objective function vector.
     */
    void set_obj(const Vector& obj);
    // TODO set_obj(int index,...)
    /**
     * \brief Set the objective function coefficient for a variable.
     *
     * \param var the variable index
     * \param value the coefficient
     */
    void set_var_obj(int var, double value);

    /**
     * \brief Set the bounds of the of the variable
     */
    void set_bounds(const IntervalVector& bounds);
    // TODO set_bounds(int index,...)
    /**
     * \brief Set the bounds on a variable
     */
    void set_var_bounds(int var, const Interval& bounds);
    void set_tolerance(double tolerance);
    void set_timeout(double timeout);
    void set_max_iter(int max_iter);

    int nb_rows() const;
    int nb_vars() const;
    double tolerance() const;
    int max_iter() const;
    double timeout() const;
    Status status() const;

    // matrix ?
    Matrix rows() const;
    Vector row(int index) const;
    Matrix rows_transposed() const;
    Vector col(int index) const;

    Vector lhs() const;
    double lhs(int index) const;
    Vector rhs() const;
    double rhs(int index) const;
    IntervalVector lhs_rhs() const;
    Interval lhs_rhs(int index) const;
    IntervalVector bounds() const;
    Interval bounds(int index) const;

    // obj
    Vector obj_vec() const;
    // obj(int)
    Vector var_obj(int index) const;

    /**
     * \brief Wether the system has a solution stored.
     */
     //TODO is_feasible
    bool has_solution() const;
    // TODO relancer automatiquement le solveur
    /**
     * \brief Return a certified enclosure for the minimum.
     */
    Interval certified_minimum() const;
    // TODO relancer automatiquement le solveur
    /**
     * \brief Return an uncertified enclosure for the minimum.
     */
    Interval uncertified_minimum() const;

    // TODO relancer automatiquement le solveur
    /**
     * \brief Return an uncertified primal solution (a minimizer).
     */
    Vector uncertified_primal_sol() const;
    // TODO relancer automatiquement le solveur
    /**
     * \brief Return an uncertified dual solution.
     */
    Vector uncertified_dual_sol() const ;

    // A voir
    bool is_point_feasible(const Vector& point) const;

    /**
     * \brief Write the linear problem to a file.
     *
     * The behaviour of this function depends on the lp-lib used.
     * For SoPlex, the format depends on the extension of the file.
     * TODO supported formats
     */
    void write_to_file(const std::string& filename) const;

    // Clear functions
    /**
     * \brief Reset the objective to zero for all variables.
     */
     // TODO set_obj_to_zero()
    void clear_obj();
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
     // TODO reset(int nb_vars)
    void reset_with_new_vars(int nb_vars);

private:
    LPSolver::Status status_{LPSolver::Status::Unknown};

    // LP solution is cached
    bool has_solution_ = false;
    Vector uncertified_primal_{1};
    Vector uncertified_dual_{1};
    Interval uncertified_obj_;

    bool has_infeasible_dir_ = false;
    Vector uncertified_infeasible_dir_{1};
    bool has_certified_obj_ = false;
    Interval certified_obj_;

    bool has_changed = true;

    // Bounds on variable in ibex::IntervalVector format.
    IntervalVector ivec_bounds_{1};

    void init(double tolerance, double timeout, int max_iter);
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