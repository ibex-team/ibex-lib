/* ============================================================================
 * I B E X - Implementation of the Interval class based on Gaol
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 23, 2011
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_GAOL_INTERVAL_H_
#define _IBEX_GAOL_INTERVAL_H_

#include "ibex_NonRecoverableException.h"

namespace ibex {

inline Interval::Interval(const gaol::interval& x) : itv(x) {

}

inline Interval& Interval::operator=(const gaol::interval& x) {
	this->itv = x;
	return *this;
}

inline Interval& Interval::operator+=(double d) {
	itv+=d;
	return *this;
}

inline Interval& Interval::operator-=(double d) {
	itv-=d;
	return *this;
}

inline Interval& Interval::operator*=(double d) {
	itv*=d;
	return *this;
}

inline Interval& Interval::operator/=(double d) {
	itv/=d;
	return *this;
}

inline Interval& Interval::operator+=(const Interval& x) {
	itv+=x.itv;
	return *this;
}

inline Interval& Interval::operator-=(const Interval& x) {
	itv-=x.itv;
	return *this;
}

inline Interval& Interval::operator*=(const Interval& x) {
	itv*=x.itv;
	return *this;
}

inline Interval& Interval::operator/=(const Interval& x) {
	//return div2_inter(*this,x);
	itv/=x.itv; // question: does Gaol perform generalized division here?
	return *this;
}

/*inline Interval Interval::operator+() const {
	return *this;
}*/

inline Interval Interval:: operator-() const {
	return -itv;
}

inline Interval& Interval::div2_inter(const Interval& x, const Interval& y) {
	Interval out2;
	div2_inter(x,y,out2);
	return *this |= out2;
}

inline void Interval::set_empty() {
	*this = EMPTY_SET;
}

inline Interval& Interval::operator&=(const Interval& x) {
	itv&=x.itv;
	return *this;
}

inline Interval& Interval::operator|=(const Interval& x) {
	itv|=x.itv;
	return *this;
}

inline double Interval::lb() const {
	return itv.left();
}

inline double Interval::ub() const {
	return itv.right();
}

inline double Interval::mid() const {
	return itv.midpoint();
}

inline bool Interval::is_subset(const Interval& x) const {
	return x.itv.set_contains(itv);
}

/* the semantic of goal does not match 
inline bool Interval::is_strict_subset(const Interval& x) const {
	return x.itv.set_strictly_contains(itv);
} */

inline bool Interval::is_superset(const Interval& x) const {
	return itv.set_contains(x.itv);
}

/* the semantic of goal does not match
inline bool Interval::is_strict_superset(const Interval& x) const {
	return itv.set_strictly_contains(x.itv);
} */

inline bool Interval::contains(double d) const {
	return itv.set_contains(d);
}

inline bool Interval::strictly_contains(double d) const {
	return itv.set_strictly_contains(d);
}

inline bool Interval::is_disjoint(const Interval &x) const {
	return itv.set_disjoint(x.itv);
}

inline bool Interval::is_empty() const {
	return itv.is_empty();
}

inline bool Interval::is_degenerated() const {
	return is_empty() || itv.is_a_double();
}

inline bool Interval::is_unbounded() const {
	return !itv.is_finite();
}


inline double Interval::diam() const {
	return itv.width();
}

inline double Interval::mig() const {
	return itv.mig();
}

inline double Interval::mag() const {
	return itv.mag();
}

inline Interval operator&(const Interval& x1, const Interval& x2) {
	return x1.itv & x2.itv;
}

inline Interval operator|(const Interval& x1, const Interval& x2) {
	return x1.itv | x2.itv;
}

inline double hausdorff(const Interval &x1, const Interval &x2) {
	return hausdorff(x1.itv,x2.itv);
}

inline Interval operator+(const Interval& x, double d) {
	return x.itv+d;
}

inline Interval operator-(const Interval& x, double d) {
	return x.itv-d;
}

inline Interval operator*(const Interval& x, double d) {
	return x.itv*d;
}

inline Interval operator/(const Interval& x, double d) {
	return x.itv/d;
}

inline Interval operator+(double d,const Interval& x) {
	return d+x.itv;
}

inline Interval operator-(double d, const Interval& x) {
	return d-x.itv;
}

inline Interval operator*(double d, const Interval& x) {
	return d*x.itv;
}

inline Interval operator/(double d, const Interval& x) {
	return d/x.itv;
}

inline Interval operator+(const Interval& x1, const Interval& x2) {
	return x1.itv+x2.itv;
}

inline Interval operator-(const Interval& x1, const Interval& x2) {
	return x1.itv-x2.itv;
}

inline Interval operator*(const Interval& x1, const Interval& x2) {
	return x1.itv*x2.itv;
}

inline Interval operator/(const Interval& x1, const Interval& x2) {
	return x1.itv/x2.itv;
}

inline Interval sqr(const Interval& x) {
	return gaol::sqr(x.itv);
}

inline Interval sqrt(const Interval& x) {
	return gaol::sqrt(x.itv);
}

inline Interval pow(const Interval& x, int n) {
	return gaol::pow(x.itv,n);
}

inline Interval pow(const Interval &x, double d) {
	return gaol::pow(x.itv, d);
}

inline Interval pow(const Interval &x, const Interval &y) {
	return gaol::pow(x.itv, y.itv);
}

inline Interval root(const Interval& x, int n) {
	return gaol::nth_root(x.itv,n);
}

inline Interval exp(const Interval& x) {
	return gaol::exp(x.itv);
}

inline Interval log(const Interval& x) {
	return gaol::log(x.itv);
}

inline Interval cos(const Interval& x) {
	return gaol::cos(x.itv);
}

inline Interval sin(const Interval& x) {
	return gaol::sin(x.itv);
}

inline Interval tan(const Interval& x) {
	return gaol::tan(x.itv);
}

inline Interval acos(const Interval& x) {
	return gaol::acos(x.itv);
}

inline Interval asin(const Interval& x) {
	return gaol::asin(x.itv);
}

inline Interval atan(const Interval& x) {
	return gaol::atan(x.itv);
}

inline Interval atan2(const Interval& x, const Interval& y) {
	return gaol::atan2(x.itv,y.itv);
}

inline Interval cosh(const Interval& x) {
	return gaol::cosh(x.itv);
}

inline Interval sinh(const Interval& x) {
	return gaol::sinh(x.itv);
}

inline Interval tanh(const Interval& x) {
	return gaol::tanh(x.itv);
}

inline Interval acosh(const Interval& x) {
	return gaol::acosh(x.itv);
}

inline Interval asinh(const Interval& x) {
	return gaol::asinh(x.itv);
}

inline Interval atanh(const Interval& x) {
	return gaol::atanh(x.itv);
}

inline Interval abs(const Interval &x) {
	return gaol::abs(x.itv);
}

inline Interval max(const Interval& x, const Interval& y) {
	return gaol::max(x.itv,y.itv);
}

inline Interval min(const Interval& x, const Interval& y) {
	return gaol::min(x.itv,y.itv);
}

inline bool proj_mul(const Interval& y, Interval& x1, Interval& x2) {
	x1 = gaol::div_rel(y.itv, x2.itv, x1.itv);
	x2 = gaol::div_rel(y.itv, x1.itv, x2.itv);
	return (!x1.is_empty()) && (!x2.is_empty());
}

inline bool proj_sqr(const Interval& y, Interval& x) {
	x = gaol::sqrt_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_pow(const Interval& y, int n, Interval& x) {
	x = gaol::nth_root_rel(y.itv, n, x.itv);
	return !x.is_empty();
}

inline bool proj_pow(const Interval& y, Interval& x1, Interval& x2) {
	throw NonRecoverableException("proj_power(y,x1,x2) (with x1 and x2 intervals) not implemented yet with Gaol");
}

inline bool proj_cos(const Interval& y,  Interval& x) {
	x = gaol::acos_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_sin(const Interval& y,  Interval& x) {
	x = gaol::asin_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_tan(const Interval& y,  Interval& x) {
	x = gaol::atan_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_atan2(const Interval& y, Interval& x1, Interval& x2) {
	throw NonRecoverableException("proj_atan2 non implemented yet with Gaol");
}

inline bool proj_cosh(const Interval& y,  Interval& x) {
	x = gaol::acosh_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_sinh(const Interval& y,  Interval& x) {
	x = gaol::asinh_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_tanh(const Interval& y,  Interval& x) {
	x = gaol::atanh_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool proj_abs(const Interval& y,  Interval& x) {
	x = gaol::invabs_rel(y.itv,x.itv);
	return !x.is_empty();
}

} // end namespace ibex

#endif // _IBEX_GAOL_INTERVAL_H_
