//============================================================================
//                                  I B E X
// File        : ibex_LPLibWrapper.cpp
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last update : Feb 13, 2025 (Gilles Chabert)
//============================================================================

#include "ibex_LPSolver.h"

namespace {

bool isfinite(const ibex::Vector& v) {
    for(int i = 0; i < v.size(); i++) {
        if(!std::isfinite(v[i])) {
            return false;
        }
    }
    return true;
}

} /* end anonymous namespace */

namespace ibex {


LPSolver::LPSolver(int nb_vars, LPSolver::Mode mode, double tolerance,
        double timeout, int max_iter)
{
    assert(nb_vars > 0);
    init(mode, tolerance, timeout, max_iter);
    //ivec_bounds_ = IntervalVector(nb_vars);
    
    reset(nb_vars);
}

LPSolver::LPSolver(std::string filename) {
    init(LPSolver::Mode::NotCertified, LPSolver::default_tolerance, LPSolver::default_timeout, LPSolver::default_max_iter);
    
    int status = myclp->readMps(filename.c_str());
    if(status) {
        std::string msg = "LPSolver: file " + filename + " could not be read.";
        ibex_error(msg.c_str());
    }
    ivec_bounds_ = IntervalVector(nb_vars());
    for(int i = 0; i < ivec_bounds_.size(); i++) {
        ivec_bounds_[i] = Interval(myclp->columnLower()[i], myclp->columnUpper()[i]);
    }
}

LPSolver::~LPSolver() {
    delete myclp;
}

void LPSolver::init(LPSolver::Mode mode, double tolerance, double timeout, int max_iter) {
    
    myclp = new ClpSimplex();
    mode_ = mode;

    // no log
    myclp->setLogLevel(0);
    /// Direction of optimization (1 - minimize, -1 - maximize, 0 - ignore
    myclp->setOptimizationDirection(1);
    
    myclp->setMaximumIterations(max_iter);
    myclp->setMaximumSeconds(timeout);
    myclp->setPrimalTolerance(tolerance);
    myclp->setDualTolerance(tolerance);
}


int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
    assert(row.size() == nb_vars());
    assert(std::isfinite(lhs) && std::isfinite(rhs));
    assert(isfinite(row));

    has_changed = true;
    
    int* row2Index = new int[nb_vars()];
    for(int i=0;i<nb_vars();i++){row2Index[i]=i;}
    try {
        myclp->addRow(nb_vars(),row2Index,row.raw(),lhs,rhs);
    }
    catch(...) {
        throw LPException();
    }
    delete [] row2Index;
    return nb_rows()-1;
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
    assert(row.size() == nb_vars());
    assert(isfinite(row));
    assert(std::isfinite(rhs));

    has_changed = true;
    
    int* row2Index = new int[nb_vars()];
    for(int i=0;i<nb_vars();i++){row2Index[i]=i;}
    try {
        if (op==LEQ || op==LT) {
            myclp->addRow(nb_vars(),row2Index,row.raw(),NEG_INFINITY,rhs);
        }
        else if (op==GEQ || op==GT) {
            myclp->addRow(nb_vars(),row2Index,row.raw(),rhs,POS_INFINITY);
        }
        else
            throw LPException();
    }
    catch(...) {
        throw LPException();
    }
    delete [] row2Index;
    
    return nb_rows()-1;
}


void LPSolver::add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs) {
    for(int i = 0; i < lhs.size(); i++) {
        add_constraint(lhs[i], rows.row(i), rhs[i]);
    }
}

void LPSolver::add_constraints(const Matrix& rows, CmpOp op, const Vector& rhs) {
    for(int i = 0; i < rhs.size(); i++) {
        add_constraint(rows.row(i), op, rhs[i]);
    }
}

LPSolver::Status LPSolver::minimize() {
    invalidate();
    assert(!ivec_bounds_.is_unbounded());
    
    // solve
    if (statistics!=NULL) statistics->add_call();
    myclp->dual();
    
    int clp_status = myclp->status();
    /** Status of problem:
                     -1 - unknown e.g. before solve or if postSolve says not optimal
                     0 - optimal
                     1 - primal infeasible
                     2 - dual infeasible
                     3 - stopped on iterations or time
                     4 - stopped due to errors
                     5 - stopped by event handler (virtual int ClpEventHandler::event())
             */
    status_ = LPSolver::Status::Unknown;
    switch(clp_status){
        case 0:
            {
                obj_ = myclp->getObjValue();
                
                // the primal solution : used by choose_next_variable
                double * primal= myclp->primalColumnSolution();
                // the dual solution ; used by Neumaier Shcherbina test
                double * dual = myclp->dualRowSolution();
                
                uncertified_primal_.resize(nb_vars());
                for(int i=0;i<nb_vars();i++){
                    uncertified_primal_[i]=primal[i];
                }
                
                uncertified_dual_.resize(nb_rows());
                for(int i=0;i<nb_rows();i++){
                    uncertified_dual_[i]=dual[i];
                }
                
                has_solution_ = true;
                if(mode_ == LPSolver::Mode::Certified) {
                    // Neumaier Shcherbina cannot fail
                    neumaier_shcherbina_postprocessing();
                    status_ = LPSolver::Status::OptimalProved;
                } else {
                    status_ = LPSolver::Status::Optimal;
                }
            }
            break;
        case 1:
            {
                status_ = LPSolver::Status::Infeasible;
                has_infeasible_dir_ = false;
                if(myclp->isProvenPrimalInfeasible() || myclp->isProvenDualInfeasible()){
                    double* ray = myclp->infeasibilityRay(true);
                    if(ray!=NULL && myclp->rayExists()){
                        has_infeasible_dir_ = true;
                        uncertified_infeasible_dir_.resize(nb_rows());
                        for(int i=0;i<nb_rows();i++){
                            uncertified_infeasible_dir_[i]=ray[i];
                        }
                        if(mode_ == LPSolver::Mode::Certified) {
                            bool infeasible_proved = neumaier_shcherbina_infeasibility_test();
                            if(infeasible_proved) {
                                status_ = LPSolver::Status::InfeasibleProved;
                            }
                        }
                    }
                }
            }
            break;
        case 3:
            status_ = LPSolver::Status::MaxIter;
            break;
        case 4:
            status_ = LPSolver::Status::Timeout;
            break;
        case 5:  case 2:
            {
                if(myclp->unboundedRay()){
                    status_ = LPSolver::Status::Unbounded;
                }
                else {
                    status_ = LPSolver::Status::Unknown;
                }
            }
            break;
        default:
            if (statistics!=NULL) statistics->add_unexpected_status(clp_status);     
            status_ = LPSolver::Status::Unknown;
    }
    //std::cout<<"Minimize done! "<<status_<<std::endl;
    return status_;
}

void LPSolver::set_cost(const Vector& obj) {
    assert(obj.size() == nb_vars());
    assert(isfinite(obj));
    has_changed = true;
    try {
        for (int i =0; i< nb_vars(); i++)
            myclp->setObjectiveCoefficient(i, obj[i]);
    }
    catch(...) {
        throw LPException();
    }
}

void LPSolver::set_cost(int index, double value) {
    assert(index >= 0 && index < nb_vars());
    assert(std::isfinite(value));
    has_changed = true;
    try {
        myclp->setObjectiveCoefficient(index, value);
    }
    catch(...) {
        throw LPException();
    }
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
    assert(bounds.size() == nb_vars());
    assert(!bounds.is_unbounded());

    has_changed = true;
    // The bounds have to be changed in 3 places: ivec_bounds_,
    // 1. ivec_bounds_
    ivec_bounds_ = bounds;
    
    // 2. in clp variable bounds
    for(int i=0;i<nb_vars();i++)
        myclp->setColumnBounds(i,ivec_bounds_[i].lb(), ivec_bounds_[i].ub());
    
    // 3. in clp bounds constraints
    for(int i = 0; i < nb_vars(); i++) {
        myclp->setRowBounds(i,bounds[i].lb(), bounds[i].ub());
    }
}

void LPSolver::set_bounds(int var, const Interval& bounds) {
    assert(var >= 0 && var < nb_vars());
    assert(!bounds.is_unbounded());
    has_changed = true;
    ivec_bounds_[var] = bounds;
    myclp->setColumnBounds(var, bounds.lb(), bounds.ub());
    myclp->setRowBounds(var,bounds.lb(), bounds.ub());
}

void LPSolver::set_tolerance(double tolerance) {
    has_changed = true;
    myclp->setPrimalTolerance(tolerance);
    myclp->setDualTolerance(tolerance);
}

void LPSolver::set_timeout(double timeout) {
    has_changed = true;
    myclp->setMaximumSeconds(timeout);
}

void LPSolver::set_max_iter(int max_iter) {
    has_changed = true;
    myclp->setMaximumIterations(max_iter);
}

int LPSolver::nb_rows() const {
    return myclp->numberRows();
}

int LPSolver::nb_vars() const {
    return myclp->numberColumns();
}

double LPSolver::tolerance() const {
    return myclp->primalTolerance();
}

int LPSolver::max_iter() const {
    return myclp->maximumIterations();
}

double LPSolver::timeout() const {
    return myclp->maximumSeconds();
}

LPSolver::Status LPSolver::status() const {
    return status_;
}

Matrix LPSolver::rows() const {
    Matrix A(nb_rows(),nb_vars(),0.0);
    try {
        CoinPackedMatrix * mat=myclp->matrix();
        if (mat->isColOrdered()) {
            for (int cc=0;cc<nb_vars(); cc++){
                CoinBigIndex j;
                CoinBigIndex end=mat->getVectorStarts()[cc]+mat->getVectorLengths()[cc];;
                for (j=mat->getVectorStarts()[cc];j<end;j++) {
                    A.row(mat->getIndices()[j])[cc] = mat->getElements()[j];
                }
            }

        } else {
            for (int rr=0;rr<nb_rows(); rr++){
                CoinBigIndex j;
                CoinBigIndex end=mat->getVectorStarts()[rr]+mat->getVectorLengths()[rr];
                for (j=mat->getVectorStarts()[rr];j<end;j++) {
                    A.row(rr)[mat->getIndices()[j]] = mat->getElements()[j];
                }
            }
        }
    } catch(...) {
        throw LPException();
    }
    return A;
}

Vector LPSolver::row(int index) const {
    assert(index >= 0 && index < nb_rows());
    if (myclp->matrix()->isColOrdered()) {
        myclp->matrix()->reverseOrdering();
    }
    Vector Ai(nb_vars(),0.0);
    
    CoinPackedMatrix * mat=myclp->matrix();
    const double* Ar = mat->getElements();
    const int* idx = mat->getIndices();
    int row_start = mat->getVectorStarts()[index];
    int nb_idx = mat->getVectorSize(index);
    for(int j=0;j<nb_idx;j++){
        Ai[idx[row_start+j]]=Ar[row_start+j];
    }
    
    return Ai;
}

Matrix LPSolver::rows_transposed() const {
    return rows().transpose();
}

Vector LPSolver::col(int index) const {
    assert(index >= 0 && index < nb_vars());
    if (!myclp->matrix()->isColOrdered()) {
        myclp->matrix()->reverseOrdering();
    }
    Vector Aj(nb_rows(),0.0);
    CoinPackedMatrix * mat=myclp->matrix();
    const double* Ac = mat->getElements();
    const int* idx = mat->getIndices();
    int col_start = mat->getVectorStarts()[index];
    int nb_idx = mat->getVectorSize(index);
    for(int i=0;i<nb_idx;i++){
        Aj[idx[col_start+i]]=Ac[col_start+i];
    }
    
    return Aj;
}

Vector LPSolver::lhs() const {
    Vector dcol(nb_rows());
    const double* lhs_=myclp->getRowLower();
    for(int i=0;i<nb_rows();i++){
        dcol[i]=lhs_[i];
    }
    return dcol;
}

double LPSolver::lhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return myclp->getRowLower()[index];
}

Vector LPSolver::rhs() const {
    Vector dcol(nb_rows());
    const double* rhs_=myclp->getRowUpper();
    for(int i=0;i<nb_rows();i++){
        dcol[i]=rhs_[i];
    }
    return dcol;
}
double LPSolver::rhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return myclp->getRowUpper()[index];
}

IntervalVector LPSolver::lhs_rhs() const {
    IntervalVector lhs_rhs_vec(nb_rows());
    for(int i = 0; i < lhs_rhs_vec.size(); i++) {
        lhs_rhs_vec[i] = Interval(myclp->getRowLower()[i], myclp->getRowUpper()[i]);
    }
    return lhs_rhs_vec;
}

Interval LPSolver::lhs_rhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return Interval(myclp->getRowLower()[index], myclp->getRowUpper()[index]);
}

IntervalVector LPSolver::bounds() const {
    return ivec_bounds_;
}

Interval LPSolver::bounds(int index) const {
    assert(index >= 0 && index < nb_vars());
    return ivec_bounds_[index];
}

Vector LPSolver::cost() const {
    Vector vec(nb_vars());
    const double* cost_=myclp->getObjCoefficients();
    for(int i=0;i<nb_vars();i++){
        vec[i]=cost_[i];
    }
    return vec;
}

double LPSolver::cost(int index) const {
    assert(index >= 0 && index < nb_vars());
    return myclp->getObjCoefficients()[index];
}

Interval LPSolver::minimum() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored. Check solver status with LPSolver::status().");
    }
    return obj_;
}

Vector LPSolver::not_proved_primal_sol() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored. Check solver status with LPSolver::status().");
    }
    return uncertified_primal_;
}
Vector LPSolver::not_proved_dual_sol() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored. Check solver status with LPSolver::status().");
    }
    return uncertified_dual_;
}

bool LPSolver::uncertified_infeasible_dir(Vector& infeasible_dir) const {
    if(has_infeasible_dir_) {
        infeasible_dir = uncertified_infeasible_dir_;
        return true;
    }
    return false;
}

void LPSolver::write_to_file(const std::string& filename) const {
    myclp->writeMps(filename.c_str());
}

// Clear functions
void LPSolver::set_cost_to_zero() {
    for(int i=0;i<nb_vars();i++){
        myclp->setObjectiveCoefficient(i,0.0);
    }
}

// remove all constraints except bound constraints
void LPSolver::clear_constraints() {
    myclp->resize(nb_vars(),nb_vars());
}

void LPSolver::clear_bounds() {
    IntervalVector new_bounds(nb_vars(), Interval::ALL_REALS);
    set_bounds(new_bounds);
}

void LPSolver::reset(int nb_vars) {
    assert(nb_vars > 0);
    invalidate();
    
    set_cost_to_zero();
    
    // define the number of variables and reset all constraints
    myclp->resize(0,nb_vars);
    
    ivec_bounds_ = IntervalVector(nb_vars, Interval::ALL_REALS);
    
    // define bounds on variables
    for(int i=0;i<nb_vars;i++){
        myclp->setColumnBounds(i,COIN_DBL_MIN, COIN_DBL_MAX);
    }
    
    // Adding explicit constraints for bounds
    Vector rowValue(nb_vars,0.0);
    for(int i = 0; i < nb_vars; i++) {
        rowValue[i]=1.0;
        add_constraint(COIN_DBL_MIN,rowValue,COIN_DBL_MAX);
        rowValue[i]=0.0;
    }
}

} /* end namespace ibex */
