#include "ibex_Exception.h"

#ifdef _WIN32
#include <float.h>
#endif

namespace ibex {

inline void fpu_round_down() {
	round_downward();
}

inline void fpu_round_up() {
	round_upward();
}

inline void fpu_round_near() {
	round_nearest();
}

inline double previous_float(double x) {
	return gaol::previous_float(x);
}

inline double next_float(double x) {
	return gaol::next_float(x);
}

//inline void fpu_round_zero() {
//	round_zero();
//}

inline Interval::Interval(const gaol::interval& x) : itv(x) {

}

inline Interval& Interval::operator=(const gaol::interval& x) {
	this->itv = x;
	return *this;
}

inline Interval& Interval::operator+=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
		itv+=d;
	return *this;
}

inline Interval& Interval::operator-=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
		itv-=d;
	return *this;

}

inline Interval& Interval::operator*=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
		itv*=d;
	return *this;
}

inline Interval& Interval::operator/=(double d) {
	if (d==POS_INFINITY || d==NEG_INFINITY)
		set_empty();
	else
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
	double m=itv.midpoint();
	fpu_round_up();
	return m;
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
	double d=itv.width();
	fpu_round_up();
	return d;
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
//	double h=hausdorff(x1.itv,x2.itv);
//	fpu_round_up();
//	return h;
	return hausdorff(x1.itv,x2.itv);
}

inline Interval operator+(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return x.itv+d;
}

inline Interval operator-(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return x.itv-d;
}

inline Interval operator*(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return x.itv*d;
}

inline Interval operator/(const Interval& x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return x.itv/d;
}

inline Interval operator+(double d,const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return d+x.itv;
}

inline Interval operator-(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return d-x.itv;
}

inline Interval operator*(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
		return d*x.itv;
}

inline Interval operator/(double d, const Interval& x) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else
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
	Interval res=gaol::sqrt(x.itv);
	fpu_round_up();
	return res;
}

inline Interval pow(const Interval& x, int n) {
	return gaol::pow(x.itv,n);
}

inline Interval pow(const Interval &x, double d) {
	if(d==NEG_INFINITY || d==POS_INFINITY)
		return Interval::EMPTY_SET;
	else {
		Interval res=gaol::pow(x.itv, d);
		fpu_round_up();
		return res;
	}
}

inline Interval pow(const Interval &x, const Interval &y) {
	Interval res=gaol::pow(x.itv, y.itv);
	fpu_round_up();
	return res;
}

inline Interval root(const Interval& x, int n) {

	// get the root of the positive part (gaol does
	// not consider negative values to be in the definition
	// domain of the root function)
	gaol::interval res = gaol::nth_root(x.itv,n>=0? n : -n);

	if (n%2==1 && x.lb()<0) {
		res |= -gaol::nth_root(-x.itv,n>=0? n : -n);
	}

	if (n<0) res = 1.0/res;

	fpu_round_up();
	return res;
}

inline Interval exp(const Interval& x) {
	Interval res = gaol::exp(x.itv);
	fpu_round_up();
	return res;
}

inline Interval log(const Interval& x) {
	if (x.ub()<=0) // gaol returns (-oo,-DBL_MAX) if x.ub()==0, instead of EMPTY_SET
		return Interval::EMPTY_SET;
	else {
		Interval res=gaol::log(x.itv);
		fpu_round_up();
		return res;
	}
}

inline Interval cos(const Interval& x) {
	Interval res = gaol::cos(x.itv);
	fpu_round_up();
	return res;
}

inline Interval sin(const Interval& x) {
	Interval res = gaol::sin(x.itv);
	fpu_round_up();
	return res;
}

inline Interval tan(const Interval& x) {
	Interval res = gaol::tan(x.itv);
	fpu_round_up();
	return res;
}

inline Interval acos(const Interval& x) {
	Interval res = gaol::acos(x.itv);
	fpu_round_up();
	return res;
}

inline Interval asin(const Interval& x) {
	Interval res = gaol::asin(x.itv);
	fpu_round_up();
	return res;
}

inline Interval atan(const Interval& x) {
	Interval res = gaol::atan(x.itv);
	fpu_round_up();
	return res;
}

inline Interval cosh(const Interval& x) {
	Interval res;
	if (x.is_unbounded()) 
		res=Interval(gaol::cosh(x.itv).left(),POS_INFINITY);
	else
		res=gaol::cosh(x.itv);
	fpu_round_up();
	return res;
}

inline Interval sinh(const Interval& x) {
	Interval res = gaol::sinh(x.itv);
	fpu_round_up();
	return res;
}

inline Interval tanh(const Interval& x) {
	Interval res = gaol::tanh(x.itv);
	fpu_round_up();
	return res;
}

inline Interval acosh(const Interval& x) {
	Interval res = gaol::acosh(x.itv);
	fpu_round_up();
	return res;
}

inline Interval asinh(const Interval& x) {
	Interval res;
	if (x.is_empty()) res=Interval::EMPTY_SET;
	else if (x.lb()>=0) res=gaol::asinh(x.itv);
	else if (x.ub()<=0) res=-gaol::asinh(-x.itv);
	else {
		gaol::interval y1=gaol::asinh(gaol::interval(0,x.ub()));
		gaol::interval y2=gaol::asinh(gaol::interval(0,-x.lb()));
		res=Interval(-y2.right(),y1.right());
	}
	return res;
}

inline Interval atanh(const Interval& x) {
	Interval res = gaol::atanh(x.itv);
	fpu_round_up();
	return res;
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

inline Interval integer(const Interval& x) {
	return gaol::integer(x.itv);
}

inline bool bwd_mul(const Interval& y, Interval& x1, Interval& x2) {
	x1 = gaol::div_rel(y.itv, x2.itv, x1.itv);
	x2 = gaol::div_rel(y.itv, x1.itv, x2.itv);
	return (!x1.is_empty()) && (!x2.is_empty());
}

inline bool bwd_sqr(const Interval& y, Interval& x) {
	x = gaol::sqrt_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool bwd_pow(const Interval& y, int expon, Interval& x) {

	// there is a bug in nth_root_rel
	//x = gaol::nth_root_rel(y.itv, expon, x.itv);

	// ---> follows a temporary copy of the code
	//      in ibex_bias_interval

	// Note: this implem assumes that we can compute root
	// with negative exponents (that is why the root function
	// has also been wrapped)

	if (expon % 2 ==0) {
		Interval proj=root(y,expon);
		Interval pos_proj= proj & x;
		Interval neg_proj = (-proj) & x;
		//std::cout << "expon=" << expon << " proj=" << proj << " x=" << x << std::endl;
		x = pos_proj | neg_proj;
	} else {
		x &= root(y, expon);
	}

	return !x.is_empty();
}

inline bool bwd_pow(const Interval& , Interval& , Interval& ) {
	ibex_error("bwd_power(y,x1,x2) (with x1 and x2 intervals) not implemented yet with Gaol");
	return false;
}

inline bool bwd_cos(const Interval& y,  Interval& x) {
	x &= gaol::acos_rel(y.itv,x.itv);
	fpu_round_up();
	return !x.is_empty();
}

inline bool bwd_sin(const Interval& y,  Interval& x) {
	/*fpu_round_up();
	interval tmp=gaol::asin_rel(y.itv,x.itv);
	if (!x.itv.set_contains(tmp)) {
		std::cout << "bug x=" << x.itv << " y=" << y.itv << " tmp=" << tmp << std::endl;
	}*/
	x &= gaol::asin_rel(y.itv,x.itv);

	return !x.is_empty();
}

inline bool bwd_tan(const Interval& y,  Interval& x) {
	x = gaol::atan_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool bwd_cosh(const Interval& y,  Interval& x) {
	x = gaol::acosh_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool bwd_sinh(const Interval& y,  Interval& x) {
	//x = gaol::asinh_rel(y.itv,x.itv);
	x &= asinh(y);
	return !x.is_empty();
}

inline bool bwd_tanh(const Interval& y,  Interval& x) {
	x = gaol::atanh_rel(y.itv,x.itv);
	return !x.is_empty();
}

inline bool bwd_abs(const Interval& y,  Interval& x) {
	x = gaol::invabs_rel(y.itv,x.itv);
	return !x.is_empty();
}

} // end namespace ibex
