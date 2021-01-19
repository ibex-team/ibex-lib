#include "ibex_LPSolver.h"

namespace ibex {

LPSolver::LPSolver(int nb_vars, LPSolver::Mode mode, double tolerance,
	double timeout, int max_iter)
{
	ibex_warning("No LP Solver available (use --lp-lib option in waf configure). ");
}

LPSolver::LPSolver(std::string filename) {
	ibex_error("LPSolver method called but LPSolver has not been configured");
}

LPSolver::~LPSolver() {
}

int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::add_constraints(const Matrix& rows, CmpOp op, const Vector& rhs) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

LPSolver::Status LPSolver::minimize() {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_cost(const Vector& obj) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_cost(int var, double value) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_bounds(int var, const Interval& bounds) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_tolerance(double tolerance) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_timeout(double timeout) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_max_iter(int max_iter) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

int LPSolver::nb_rows() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

int LPSolver::nb_vars() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

double LPSolver::tolerance() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

int LPSolver::max_iter() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

double LPSolver::timeout() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

LPSolver::Status LPSolver::status() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Matrix LPSolver::rows() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::row(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Matrix LPSolver::rows_transposed() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::col(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::lhs() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

double LPSolver::lhs(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::rhs() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

double LPSolver::rhs(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

IntervalVector LPSolver::lhs_rhs() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Interval LPSolver::lhs_rhs(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

IntervalVector LPSolver::bounds() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Interval LPSolver::bounds(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::cost() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

double LPSolver::cost(int index) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Interval LPSolver::minimum() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::not_proved_primal_sol() const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

Vector LPSolver::not_proved_dual_sol() const  {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

bool LPSolver::uncertified_infeasible_dir(Vector& infeasible_dir) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::write_to_file(const std::string& filename) const {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::set_cost_to_zero() {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::clear_constraints() {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::clear_bounds() {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

void LPSolver::reset(int nb_vars) {
	ibex_error("LPSolver method called but no LPSolver has been configured");
}

} /* end namespace ibex */
