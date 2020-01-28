#include "ibex_LPSolver.h"

namespace ibex {

LPSolver::LPSolver(int nb_vars, LPSolver::Mode mode, double tolerance,
	double timeout, int max_iter)
{
	ibex_warning("No LP Solver available (use --lp-lib option in waf configure). ");
}

LPSolver::LPSolver(std::string filename) {
	throw LPException();
}

LPSolver::~LPSolver() {
}

int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
	throw LPException();
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
	throw LPException();
}

void LPSolver::add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs) {
	throw LPException();
}

void LPSolver::add_constraints(const Matrix& rows, CmpOp op, const Vector& rhs) {
	throw LPException();
}

LPSolver::Status LPSolver::minimize() {
	throw LPException();
}

void LPSolver::set_cost(const Vector& obj) {
	throw LPException();
}

void LPSolver::set_cost(int var, double value) {
	throw LPException();
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
	throw LPException();
}

void LPSolver::set_bounds(int var, const Interval& bounds) {
	throw LPException();
}

void LPSolver::set_tolerance(double tolerance) {
	throw LPException();
}

void LPSolver::set_timeout(double timeout) {
	throw LPException();
}

void LPSolver::set_max_iter(int max_iter) {
	throw LPException();
}

int LPSolver::nb_rows() const {
	throw LPException();
}

int LPSolver::nb_vars() const {
	throw LPException();
}

double LPSolver::tolerance() const {
	throw LPException();
}

int LPSolver::max_iter() const {
	throw LPException();
}

double LPSolver::timeout() const {
	throw LPException();
}

LPSolver::Status LPSolver::status() const {
	throw LPException();
}

Matrix LPSolver::rows() const {
	throw LPException();
}

Vector LPSolver::row(int index) const {
	throw LPException();
}

Matrix LPSolver::rows_transposed() const {
	throw LPException();
}

Vector LPSolver::col(int index) const {
	throw LPException();
}

Vector LPSolver::lhs() const {
	throw LPException();
}

double LPSolver::lhs(int index) const {
	throw LPException();
}

Vector LPSolver::rhs() const {
	throw LPException();
}

double LPSolver::rhs(int index) const {
	throw LPException();
}

IntervalVector LPSolver::lhs_rhs() const {
	throw LPException();
}

Interval LPSolver::lhs_rhs(int index) const {
	throw LPException();
}

IntervalVector LPSolver::bounds() const {
	throw LPException();
}

Interval LPSolver::bounds(int index) const {
	throw LPException();
}

Vector LPSolver::cost() const {
	throw LPException();
}

double LPSolver::cost(int index) const {
	throw LPException();
}

bool LPSolver::is_feasible() const {
	throw LPException();
}

Interval LPSolver::minimum() const {
	throw LPException();
}

Vector LPSolver::not_proved_primal_sol() const {
	throw LPException();
}

Vector LPSolver::not_proved_dual_sol() const  {
	throw LPException();
}

void LPSolver::write_to_file(const std::string& filename) const {
	throw LPException();
}

void LPSolver::set_cost_to_zero() {
	throw LPException();
}

void LPSolver::clear_constraints() {
	throw LPException();
}

void LPSolver::clear_bounds() {
	throw LPException();
}

void LPSolver::reset(int nb_vars) {
	throw LPException();
}

} /* end namespace ibex */
