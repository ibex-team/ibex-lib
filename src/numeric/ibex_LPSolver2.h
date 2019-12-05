#ifndef __IBEX_LP_SOLVER_H__
#define __IBEX_LP_SOLVER_H__

#include <vector>

namespace ibex {

class LPSolver {

public:

    /** the default precision on the objective, set to 1e-10. */
	static constexpr double default_tolerance = 1e-9;

    /** Default max_time_out, set to 100s  */
    static constexpr double default_timeout = 100;

    /** Default max_iter, set to 100 iterations */
    static constexpr int default_max_iter = 100;

    LPSolver(int vars_count=0, Sense sense=Sense::Minimize, double tolerance=default_tolerance,
        double timeout=default_timeout, int max_iter=default_max_iter);
    ~LPSolver();

    enum class Status { Optimal, OptimalProved, Infeasible, InfeasibleProved, Timeout, MaxIter, Unbounded, Unknown };
    enum class Sense { Maximize, Minimize };
    enum class PostProcessing { None, NeumaierShcherbina };

    void add_variable(const Interval& bounds, double obj);
    void add_variable(const Vector& col, const Interval& bounds, double obj);
    void add_variables(const IntervalVector& bounds, const Vector& obj);
    void add_variables(const Matrix& cols, const IntervalVector& bounds, const Vector& obj);

    int add_constraint(double lhs, const Vector& row, double rhs);
    int add_constraint(const Vector& row, CmpOp op, double rhs);
    void add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs);
    void add_constraints(const Matrix& rows, const std::vector<CmpOp>& ops, const Vector& rhs);

    Status optimize(PostProcessing pp = PostProcessing::NeumaierShcherbina);

    void set_sense(LPSolver::Sense sense);
    void set_obj(const Vector& obj);
    void set_var_obj(int index, double value);
    void set_bounds(const IntervalVector& bounds);
    void set_var_bounds(int var, const Interval& bounds);
    void set_tolerance(double tolerance);
    void set_timeout(double timeout);
    void set_max_iter(int max_iter);


    int rows_count() const;
    int vars_count() const;
    double tolerance() const;
    int max_iter() const;
    double timeout() const;
    Status status() const;

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

    Vector obj() const;
    Vector var_obj(int index) const;

    bool has_solution() const;
    Vector certified_obj() const;
    Vector uncertified_obj() const;

    Vector uncertified_primal_sol() const;
    Vector uncertified_dual_sol() const ;

    bool uncertified_infeasible_dir(Vector& infeasible_dir) const;

    bool is_inner_point(const Vector& point) const;

    void write_to_file(const std::string& filename) const;

    // Clear functions
    void clear_obj();
    void clear_constraints();
    void clear_bounds();


private:
    LPSolver::Sense sense_;
    LPSolver::Status status_;

    // LP solution is cached
    bool has_solution_ = false;
    Vector uncertified_primal_(1);
    Vector uncertified_dual_(1);
    double uncertified_obj_;

    Vector* uncertified_infeasible_dir_ = nullptr;
    double* certified_obj_ = nullptr;

    IntervalVector ivec_bounds_(1);

    bool neumaier_shcherbina_postprocessing();
    bool neumaier_shcherbina_infeasibility_test();

    void invalidate();

    void add_bound_constraint(int var, const Interval& bounds);

    /* This is a macro that should be defined in ibex_LPLibWrapper.h */
	IBEX_LPSOLVER_WRAPPER_ATTRIBUTES;

};

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LPSolver::LPSolverStatus x);

} // end namespace ibex

#endif /* __IBEX_LP_SOLVER_H__ */