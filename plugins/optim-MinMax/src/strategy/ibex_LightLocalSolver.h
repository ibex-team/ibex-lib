#ifndef IBEX_LIGHTLOCALSOLVER_H
#define IBEX_LIGHTLOCALSOLVER_H

#include <vector>
#include <queue>
#include "ibex_Bsc.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_Cell.h"
#include "ibex_Heap.h"
#include "ibex_OptimData.h"
#include <fstream>
#include <string>
#include "ibex_UnconstrainedLocalSearch.h"
#include "ibex_AffineEval.h"
#include "ibex_DataMinMax.h"
#include "ibex_CellCostFunc.h"
#include "ibex_LargestFirst.h"
#include "ibex_Linear.h"
#include "ibex_Ctc.h"

namespace ibex {



class LightLocalSolver
{
public:
    LightLocalSolver(NormalizedSystem& y_sys,UnconstrainedLocalSearch* local_solver_over_x,UnconstrainedLocalSearch* local_solver_over_y,Ctc& x_ctc,int x_dim,int y_dim,bool csp_actif);
    ~LightLocalSolver();

    bool compute_supy_lb(Cell* x_cell,double uplo,double loup,Function* minus_goal);

    double min_acpt_diam;  // min max_diam of local supy sols to consider it as unique point (no regression needed)

    int nb_sols;

    int x_dim;

    int y_dim;

    int nb_sivia_iter;

    int nb_optim_iter;

    double y_sol_radius;

    double reg_acpt_error;

    Ctc& x_ctc;

    std::queue< std::pair<IntervalVector,Interval> > csp_queue;

    double perf_ctc_coef; // performance of <loup csp between 0 and 1

    int nb_rdm_candidates;


private:
    friend class OptimMinMax;

    std::vector<Vector> supy_local_sols(const Vector& global_xy_sol,Cell* x_cell);

    IntervalVector search_box_over_y(const Vector& xy_start_point);

    IntervalVector search_box_over_x(const Vector& xy_start_point,Cell* x_cell);

    IntervalVector search_box_over_xy(Cell* x_cell);

    Vector xy_stpt(const Vector& global_xy_sol,Cell* x_cell);

    IntervalVector hull_ysol_box(const Vector& global_xy_sol, std::vector<Vector> local_sols);

    Vector sols_std_dev(const std::vector<Vector>& local_sols);

    Vector sols_mean(const std::vector<Vector>& local_sols);

    std::vector<Vector> reject_far_y_sol(const Vector& global_sol,std::vector<Vector> local_sols);

    std::queue<std::pair<IntervalVector,Interval> > sivia_x_bsct(Cell* x_cell,const std::pair<std::vector<Vector>,std::vector<Matrix> >& local_solutions, double loup) ;

    IntervalVector list_hull_box(std::queue<std::pair<IntervalVector,Interval> > list);

    double list_volume(std::queue<std::pair<IntervalVector,Interval> > list);

    double compute_lb(std::queue<std::pair<IntervalVector,Interval> > x_list,const std::pair<std::vector<Vector>,std::vector<Matrix> >& local_solutions,Cell* x_cell,double loup);

    IntervalVector set_x_box_y_vect(const IntervalVector& x_box, const Vector& y_vect);

    IntervalVector set_x_box_y_box(const IntervalVector& x_box, const IntervalVector& y_vect);

    std::pair<Matrix,Vector> affine_regression(const std::vector<Vector>& local_sols);

    std::vector<Vector> find_max_candidates(double uplo,Cell* x_cell);

    bool check_twins(const Vector& v,const std::vector<Vector>& list);

    std::pair<std::vector<Vector>,std::vector<Matrix> > get_local_solutions(std::vector<Vector> candidates,double loup,Cell* x_cell);

    Interval eval_backward_max_solutions(const std::pair<std::vector<Vector>,std::vector<Matrix> >& local_solutions,IntervalVector& x_box,double loup);


    Heap<Cell> *x_heap;

    UnconstrainedLocalSearch *local_solver_over_y;
    UnconstrainedLocalSearch *local_solver_over_x;
    bool csp_actif;

//    std::vector<Vector> local_sols;
//    std::vector<Matrix> regression_matrix;
//    std::vector<Vector> point_solution;
    NormalizedSystem& xy_sys; // contains constraints on x and y

    static CellCostPFlb x_heap_costf;


    static const double default_nb_sols;
    static const double default_min_acpt_diam;
    static const int default_nb_sivia_iter;
    static const int default_nb_optim_iter;
    static const double default_y_sol_radius;
    static const double default_reg_acpt_error;


};

std::pair<Vector,double> fit_line(std::queue<double>& observ);

//std::pair<Matrix,Matrix> lu_decompose(Matrix sqr_matrix);
//Matrix pivotize(const Matrix& M);
//void PLU_decompose(const Matrix& A,Matrix& P,Matrix& L,Matrix& U);
//Matrix inverse(const Matrix& A);
//Vector luEvaluate(const Matrix& L, const Matrix& U, const Vector& b);


}

#endif // IBEX_LIGHTLOCALSOLVER_H
