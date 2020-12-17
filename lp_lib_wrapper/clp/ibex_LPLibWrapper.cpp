#include "ibex_LPSolver.h"

namespace {

bool isfinite(const ibex::Vector& v) {
    for(int i = 0; i < v.size(); ++i) {
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
    for(int i = 0; i < ivec_bounds_.size(); ++i) {
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
    myclp->setOptimizationDirection(1)
    
    myclp->setMaximumIterations(max_iter);
    myclp->setMaximumSeconds(timeout);
    myclp->setPrimalTolerance(tolerance);
    myclp->setDualTolerance(tolerance);
}

/*void LPSolver::add_variable(const Interval& bounds, double obj) {
    Vector col(nb_rows(), 0.0);
    return add_variable(col, bounds, obj);
}

void LPSolver::add_variable(const Vector& col, const Interval& bounds, double obj) {
    assert(col.size() == nb_vars());
    const int n = nb_vars();
    DSVectorReal dscol = ivec2dsvec(col);
    mysoplex->addColReal(LPColReal(obj, dscol, bounds.ub(), bounds.lb()));

    ivec_bounds_.resize(n+1);
    ivec_bounds_[n] = bounds;

    // Add bound constraint to the LP
    bounds_ctrs_.emplace_back(-1);
    add_bound_constraint(n, bounds);
}

void LPSolver::add_variables(const IntervalVector& bounds, const Vector& obj) {
    assert(obj.size() == bounds.size());
    for(int i = 0; i < bounds.size(); ++i) {
        add_variable(bounds[i], obj[i]);
    }
}

void LPSolver::add_variables(const Matrix& cols, const IntervalVector& bounds, const Vector& obj) {
    for(int i = 0; i < bounds.size(); ++i) {
        add_variable(cols.col(i), bounds[i], obj[i]);
    }
}*/

int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
    assert(row.size() == nb_vars());
    assert(std::isfinite(lhs) && std::isfinite(rhs));
    assert(isfinite(row));

    has_changed = true;
    
    try {
        myclp->addRow(nb_vars,_col1Index,&(row[0]),lhs,rhs);
        nb_rows++;
    }
    catch(...) {
        throw LPException();
    }
    return nb_rows()-1;
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
    assert(row.size() == nb_vars());
    assert(isfinite(row));
    assert(std::isfinite(rhs));

    has_changed = true;
    
    try {
        if (sign==LEQ || sign==LT) {
            myclp->addRow(nb_vars,_col1Index,row.raw(),NEG_INFINITY,rhs);
            nb_rows++;
        }
        else if (sign==GEQ || sign==GT) {
            myclp->addRow(nb_vars,_col1Index,row.raw(),rhs,POS_INFINITY);
            nb_rows++;
        }
        else
            throw LPException();
    }
    catch(...) {
        throw LPException();
    }
    
    return nb_rows()-1;
}

/*void LPSolver::add_bound_constraint(int var, const Interval& bounds) {
    assert(var >= 0 && var < nb_vars());
    DSVectorReal dsrow(nb_vars());
    dsrow.add(var, 1);
    mysoplex->addRowReal(LPRowReal(bounds.lb(), dsrow, bounds.ub()));
    bounds_ctrs_[var] = nb_rows()-1;
}*/

void LPSolver::add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs) {
    for(int i = 0; i < lhs.size(); ++i) {
        add_constraint(lhs[i], rows.row(i), rhs[i]);
    }
}

void LPSolver::add_constraints(const Matrix& rows, CmpOp op, const Vector& rhs) {
    for(int i = 0; i < rhs.size(); ++i) {
        add_constraint(rows.row(i), op, rhs[i]);
    }
}

LPSolver::Status LPSolver::minimize() {
    invalidate();
    assert(!ivec_bounds_.is_unbounded());

    myclp->dual();
    ClpSimplex::Status clp_status;
    clp_status = myclp->getStatus();
    
    status_ = LPSolver::Status::Unknown;
    if (myclp->isProvenOptimal()) {
        obj_ = myclp->getObjValue();
        
        // the primal solution : used by choose_next_variable
        double * primal= myclp->primalColumnSolution();
        // the dual solution ; used by Neumaier Shcherbina test
        double * dual = myclp->dualRowSolution();
        
        uncertified_primal_ = Vector(nb_vars());
        for(int i=0;i<nb_vars();i++){
            uncertified_primal_[i]=primal[i];
        }
        
        uncertified_dual_ = Vector(nb_rows());
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
    else if (myclp->isProvenPrimalInfeasible()){
        status_ = LPSolver::Status::Timeout;
    }
    else if (myclp->isIterationLimitReached()){
        status_ = LPSolver::Status::MaxIter;
    }
    else if (myclp->isProvenPrimalInfeasible()){
        status_ = LPSolver::Status::Infeasible;
    }
    else if (myclp->infeasibilityRay()!=NULL){
        status_ = LPSolver::Status::Unbounded;
    }
    else{
    	// TODO: we need an option to log such warnings, see issue #440.
        status_ = LPSolver::Status::Unknown;
    }
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
    // in soplex variable bounds, and in bounds constraints.
    ivec_bounds_ = bounds;
    
    const int n = nb_vars();
    for(int i = 0; i < n; ++i) {
        myclp->setColumnBounds(i, bounds[i].lb(), bounds[i].ub());
    }
}

void LPSolver::set_bounds(int var, const Interval& bounds) {
    assert(var >= 0 && var < nb_vars());
    assert(!bounds.is_unbounded());
    has_changed = true;
    ivec_bounds_[var] = bounds;
    myclp->setColumnBounds(var, bounds.lb(), bounds.ub());
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
    Matrix A(nb_rows,nb_vars,0);
    try {
        CoinPackedMatrix * mat=myclp->matrix();
        // see mat.getCorefficient()
        //A = Matrix::zeros(nb_rows,nb_vars);
        if (mat->isColOrdered()) {
            for (int cc=0;cc<nb_vars; cc++){
                CoinBigIndex j;
                CoinBigIndex end=mat->getVectorStarts()[cc]+mat->getVectorLengths()[cc];;
                for (j=mat->getVectorStarts()[cc];j<end;j++) {
                    A.row(mat->getIndices()[j])[cc] = mat->getElements()[j];
                }
            }

        } else {
            for (int rr=0;rr<nb_rows; rr++){
                CoinBigIndex j;
                CoinBigIndex end=mat->getVectorStarts()[rr]+mat->getVectorLengths()[rr];;
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
    bool tr=false;
    if (myclp->matrix()->isColOrdered()) {
        myclp->matrix()->transpose();
        tr=true;
    }
    Vector Ai(nb_vars(),0);
    const CoinShallowPackedVector& rowi = myclp->matrix()->getVector(i);
    const int* idx=rowi.getIndices();
    for(int j=0;j<rowi.getNumElements();j++){
        Ai[idx[j]] = rowi.getElements()[j];
    }
    if(tr){
        myclp->matrix()->transpose();
    }
    return Ai;
}

Matrix LPSolver::rows_transposed() const {
    return rows().transpose();
}

Vector LPSolver::col(int index) const {
    assert(index >= 0 && index < nb_vars());
    bool tr=false;
    if (!myclp->matrix()->isColOrdered()) {
        myclp->matrix()->transpose();
        tr=true;
    }
    Vector Aj(nb_rows(),0);
    const CoinShallowPackedVector& colj =myclp->matrix()->getVector(index);
    const int* idx=colj.getIndices();
    for(int i=0;i<colj.getNumElements();i++){
        Aj[idx[i]] = colj.getElements()[i];
    }
    if(tr){
        myclp->matrix()->transpose();
    }
    return col;
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
    for(int i = 0; i < lhs_rhs_vec.size(); ++i) {
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
    const double* rhs_=myclp->getObjCoefficients();
    for(int i=0;i<nb_rows();i++){
        dcol[i]=rhs_[i];
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
        myclp->setObjCoefficients(i,0);
    }
}

void LPSolver::clear_constraints() {
    myclp->createEmptyMatrix();
}

void LPSolver::clear_bounds() {
    IntervalVector new_bounds(nb_vars(), Interval::ALL_REALS);
    set_bounds(new_bounds);
}

void LPSolver::reset(int nb_vars) {
    assert(nb_vars > 0);
    invalidate();
    
    clear_bounds();
    clear_constraints();
    set_cost_to_zero();
    
    
    for(int i = 0; i < nb_vars; ++i) {
        Vector dsrow(nb_vars(),0.0);
        dsrow[i]=1;
        myclp->addRow(nb_vars,i,dsrow.raw(),NEG_INFINITY,POS_INFINITY);
    }
    ivec_bounds_ = IntervalVector(nb_vars, Interval::ALL_REALS);
}

} /* end namespace ibex */
