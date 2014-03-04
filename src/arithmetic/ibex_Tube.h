/* ============================================================================
 * I B E X - Tube definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Aymeric Bethencourt,
 *               Jordan Ninin, Gilles Chabert
 * Created     : Oct 7, 2013
 * ---------------------------------------------------------------------------- */

#define __IBEX_TUBE_H__
#ifndef __IBEX_TUBE_H__  // The class Tube is not ready !!!

#include <cassert>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Function.h"


namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Tube
 *
 */
class Tube : public IntervalVector {
private:

	double 			_t0;
	double 			_tf;
	double 			_deltaT;

public:

	/**
	 * \brief Create a tube with default initialization.
	 *
	 * Create a Tube over an interval [t0,tf].
	 *
	 * <p>
	 * A tube represents an unkwon real-valued function f of one variable t.
	 * If we assimilate this variable to the time, \d t0 is the
	 * "initial time" and \d tf the "final time".
	 *
	 * <p>
	 * The function is represented numerically by a vector of intervals.
	 * The time discretization step is equal to \a step.
	 *
	 * <p>
	 * All the components are initialized to \a x.
	 *
	 * \param t0 - initial time
	 * \param tf - final time
	 * \param step - time step
	 */
	Tube(double t0, double tf, double step, const Interval& x=Interval::ALL_REALS);

	/**
	 * \brief Create a tube from an initial vector.
	 *
	 * \see comments in #Tube(double, double, double, const Interval&).
	 *
	 * Each component is initialized to the corresponding component in \a x.
	 *
	 */
	Tube(double t0, double tf, double step, const IntervalVector& x);

	/**
	 * \brief Create a tube from an initial array.
	 *
	 * \see comments in #Tube(double, double, double, const Interval&).
	 *
	 * Each component is initialized to the corresponding component in \a bounds.
	 *
	 */
	Tube(double t0, double tf, double step, double bounds[][2]);

	/**
	 * \brief Create a tube from an initial array.
	 *
	 * \see comments in #Tube(double, double, double, const Interval&).
	 *
	 * Each component is initialized to the corresponding component in \a x.
	 *
	 */
	Tube(double t0, double tf, double step, const Vector& x);

	/**
	 * \brief Create a tube from bound functions.
	 *
	 * \see comments in #Tube(double, double, double, const Interval&).
	 *
	 * Each component is initialized to ??? TODO: add comment
	 *
	 */
	Tube(double t0, double tf, double pas, const Function& fmin, const Function& fmax);

	/**
	 * \brief Create [empty; ...; empty]
	 *
	 * \see comments in #Tube(double, double, double, const Interval&).
	 *
	 * All the components are initialized to the empty interval.
	 *
	 * \pre n>0
	 */
	static Tube empty(double t0, double tf, double pas);

	/**
	 * \brief Delete this vector
	 */
	virtual ~Tube() {};

	/**
	 * \brief Resize this Tube by changing the discretization step
	 */
    void resample(double new_deltaT);

	/**
	 * \brief Return a subTube.
	 *
	 * \pre (*this) must not be empty and (t0> _t0) and (tf<_tf)
	 * \return [ (*this)[t0]; ...; (*this)[tf] ].
	 */
	Tube sub_tube(double t0, double tf) const;

	/**
	 * \brief Assign this Tube to x.
	 *
	 * \pre Dimensions of this and x must match.
	 * \note Emptiness is overridden.
	 */
	Tube& operator=(const Tube& x);

	/**
	 * \brief Assign this Tube to x.
	 *
	 * \pre Dimensions of this and x must match.
	 * \note Emptiness is overridden.
	 */
	Tube& operator=(const IntervalVector& x);

	/**
	 * \brief Set *this to its intersection with x
	 *
	 * \return a reference to this.
	 * \throws InvalidVectorOp if the vectors do not have the same dimensions.
	 */
	Tube& operator&=(const Tube& x);

	/**
	 * \brief Set this Tube to the hull of itself and another.
	 *
	 * \return a reference to this.
	 * \throws InvalidVectorOp if Tubes do not have the same dimensions.
	 */
	Tube& operator|=(const Tube& x);

	/**
	 * \brief Return the intersection of this and x.
	 */
	Tube operator&(const Tube& x) const;

	/**
	 * \brief Return the hull of this & x.
	 */
	Tube operator|(const Tube& x) const;

	/** \brief (*this)+=x2.
	 * TODO: I would prefere this function to be named shift_y(doule x2) */
	Tube& operator+=(double x2);
	/** \brief (*this)+=x2.
	 * TODO: I would prefere this function to be named differently.
	 * e.g.: shift_y, add_cst, ... */
	Tube& operator+=(const Interval& x2);
	/** \brief (*this)+=x2. */
	Tube& operator+=(const Tube& x2);

	/** \brief (*this)-=x2. */
	Tube& operator-=(double x2);
	/** \brief (*this)-=x2.
	 * TODO: I would prefere this function to be named differently. */
	Tube& operator-=(const Interval& x2);
	/** \brief (*this)-=x2. */
	Tube& operator-=(const Tube& x2);

	/** \brief (*this)*=x2 */
	Tube& operator*=(double x2);
	/** \brief (*this)*=x2 */
	Tube& operator*=(const Interval& x2);
	/** \brief (*this)*=x2 */
	Tube& operator*=(const Tube& x2);

	/** \brief (*this)*=x2 */
	Tube& operator/=(double x2);
	/** \brief (*this)*=x2 */
	Tube& operator/=(const Interval& x2);
	/** \brief (*this)*=x2 */
	Tube& operator/=(const Tube& x2);

	/**
	 * \brief Get initial time.
	 */
	double get_t0() const;

	/**
	 * \brief Get final time.
	 */
	double get_tF() const;

	/**
	 * \brief Get the time discretization step.
	 */
	double get_delta_t() const;

	/**
	 * \brief Set initial time.
	 */
	void set_t0(double t0, Interval inter);

	/**
	 * \brief Set initial time.
	 */
	void set_t0(double t0);

	/**
	 * \brief Set final time.
	 */
	void set_tF(double tf, Interval inter);

	/**
	 * \brief Set final time.
	 */
	void set_tF(double tf);

	/**
	 * \brief Return f(t).
	 *
	 * \return a const reference to the value of the function at t=ti
	 *
	 * \pre t0<=t<=tf.
	 */
	const Interval& at(double ti) const;

	/**
	 * \brief Return f(t).
	 *
	 * \return a reference to the value of the function at t=ti
	 *
	 * \pre t0<=t<=tf.
	 */
	Interval& at(double ti);


	/**
	 * \brief Return f([t])
	 *
	 * \return an enclosure of the function for the variable varying in [t]
	 *
	 * \pre t0<=t<=tf.
	 */
    Interval at(const Interval &t) const;
//
//	/**
//	 * \brief Return f(t_i)
//	 *
//	 * \return a const reference to the value of the function at the i^th time step.
//	 *
//	 * \pre 0<=i<=size()
//	 */
//	const Interval& operator[](int i) const;
//
//	/**
//	 * \brief Return f(t_i)
//	 *
//	 * \return a const reference to the value of the function at the i^th time step.
//	 *
//	 * \pre 0<=i<=size()
//	 */
//
//	Interval& operator[](int i);

	/* \brief Return the maximal value of the tube (thus part of the upper bound).
	 *
	 *  \throws InvalidVectorOp if the tube is empty.
	 */
	double max() const;

	/**
	 * \brief Return the minimal value of the tube (thus part of the lower bound).
	 *
	 * \throws InvalidVectorOp if the tube is empty.
	 */
	double min() const;

    /**
     * \brief Contract *this at time \a time to interval \a in .
     */
    Tube& ctcIn(double time, const Interval& in);

    /**
     * \brief Contract *this at time \a time to interval \a in .
     */
    Tube& ctcIn(const Interval& time, const Interval& in);

    /**
     * \brief Contract *this to its intersection with tube \a x .
     */
    Tube& ctcInter(const Tube& x);

    /**
     * \brief Contract *this to its union with tube \a x .
     */
    Tube& ctcUnion(const Tube& x);

    /**
     * \brief Contract *this according to a constraint on the trajectory in \a x .
     */
    Tube& ctcEq(const Tube& x);
    Tube& ctcSup(const Tube& x);
    Tube& ctcInf(const Tube& x);

	/**
     * \brief Contract *this forward and/or backward according to the state equation \a xpoint=f(x) using an euler method.
     * Notice that we loose the interval guarantee. Please use VNODE for guaranted results.
	 */
    Tube& ctcFwd(const Function &f);
    Tube& ctcBwd(const Function &f);
    Tube& ctcFwdBwd(const Function &f);

	/**
     *\brief Contract a periodic tube with a \a period
	 */
    Tube& ctcPeriodic(double period);

	/**
     * \brief Contract the tube with a central symetric constraint
	 */
    Tube& ctcCSym(double pivot);


	/**
     * \brief Contract the tube with an axial symetric constraint
	 */
    Tube& ctcASym(double pivot);

	/**
     * \brief Return the tube delayed of \a delay seconds
	 */
    Tube& shift(double delay);

	/**
     * \brief Return the tube scaled to a \a coeff
	 */
    Tube& scale(double coef);

    /**
     * \brief Return the integral of the  tube
     */
    Interval integral(const int kmin, const int kmax);

    /**
     * \brief Return the integral of the  tube
     */
    Tube& integral();
};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief -x.
 */
Tube operator-(const Tube& x);

/** \brief x1+x2. */
Tube operator+(const Tube& x1, const Tube& x2);
/** \brief x1+x2. */
Tube operator+(const Tube& x1, double x2);
/** \brief x1+x2. */
Tube operator+(double x1, const Tube& x2);
/** \brief x1+x2. */
Tube operator+(const Tube& x1, const Interval& x2);
/** \brief x1+x2. */
Tube operator+(const Interval& x1, const Tube& x2);


/** \brief x1-x2. */
Tube operator-(const Tube& x1, const Tube& x2);
/** \brief x1-x2. */
Tube operator-(const Tube& x1, double x2);
/** \brief x1-x2. */
Tube operator-(double x1, const Tube& x2);
/** \brief x1-x2. */
Tube operator-(const Tube& x1,  const Interval& x2);
/** \brief x1-x2. */
Tube operator-(const Interval& x1, const Tube& x2);

/** \brief x1*x2. */
Tube operator*(double x1, const Tube& x2);
/** \brief x1*x2. */
Tube operator*(double x1, const Tube& x2);
/** \brief x1*x2. */
Tube operator*(const Tube& x1, double x2);
/** \brief x1*x2. */
Tube operator*(const Interval& x1, const Tube& x2);
/** \brief x1*x2. */
Tube operator*(const Tube& x1, const Interval& x2);

/** \brief x1/x2. */
Tube operator/(double x1, const Tube& x2);
/** \brief x1/x2. */
Tube operator/(double x1, const Tube& x2);
/** \brief x1/x2. */
Tube operator/(const Tube& x1, double x2);
/** \brief x1/x2. */
Tube operator/(const Interval& x1, const Tube& x2);
/** \brief x1/x2. */
Tube operator/(const Tube& x1, const Interval& x2);

/** \brief [x]^2 */
Tube sqr(const Tube& x);

/** \brief sqrt{[x]} */
Tube sqrt(const Tube& x);

/** \brief [x]^n. */
Tube pow(const Tube& x, int n);

/** \brief [x]^d. */
Tube pow(const Tube& x, double d);

/** \brief [x]^[y]. */
Tube pow(const Tube &x, const Interval &y);

/** \brief n^{th} root of [x]. */
Tube root(const Tube& x, int n);

/** \brief exp([x]). */
Tube exp(const Tube& x);

/** \brief log([x]). */
Tube log(const Tube& x);

/** \brief cos([x]). */
Tube cos(const Tube& x);

/** \brief sin([x]). */
Tube sin(const Tube& x);

/** \brief tan([x]). */
Tube tan(const Tube& x);

/** \brief acos([x]). */
Tube acos(const Tube& x);

/** \brief asin([x]). */
Tube asin(const Tube& x);

/** \brief atan([x]). */
Tube atan(const Tube& x);

/** \brief atan2([x],[y]). */
Tube atan2(const Tube& x, const Tube& y);

/** \brief cosh([x]). */
Tube cosh(const Tube& x);

/** \brief sinh([x]). */
Tube sinh(const Tube& x);

/** \brief tanh([x]). */
Tube tanh(const Tube& x);

/** \brief acosh([x]). */
Tube acosh(const Tube& x);

/** \brief asinh([x]). */
Tube asinh(const Tube& x);

/** \brief atanh([x]). */
Tube atanh(const Tube& x);

/**
 * \brief |x|.
 */
Tube abs(const Tube& x);

/**
 * \brief Return the tube f(x)
 */
Tube eval(const Function &f, const Tube &x);

/**
 * \brief Return the tube  $f^{-1}(x)$
 */
Tube inverse(const Function &f, const Tube &x);

/**
 * \brief Display the Tube \a x
 */
std::ostream& operator<<(std::ostream& os, const Tube& x);


/*@}*/

/*============================================ inline implementation ============================================ */

#define __assert_tube_time_domain__(t1,t2) assert(((t1).get_t0()==(t2).get_t0()) \
											   && ((t1).get_tF()==(t2).get_tF()) \
											   && ((t1).get_delta_t()==(t2).get_delta_t()))

inline Tube Tube::empty(double t0, double tf, double deltaT) {
	return Tube(t0,tf,deltaT,Interval::EMPTY_SET);
}

inline double Tube::get_t0() const {
	return _t0;
}

inline double Tube::get_tF() const {
	return _tf;
}

inline double Tube::get_delta_t() const {
	return _deltaT;
}

inline const Interval& Tube::at(double t) const {
	return (*this)[(int)((t-_t0)/_deltaT)];
}

inline Interval& Tube::at(double t) {
	return (*this)[(int)((t-_t0)/_deltaT)];
}

inline Tube& Tube::ctcEq(const Tube& x) {
	return (*this).ctcInter(x);
}

inline Tube Tube::operator&(const Tube& x) const {
	return Tube(*this) &= x;
}

inline Tube Tube::operator|(const Tube& x) const {
	return Tube(*this) |= x;
}

inline Tube& Tube::operator+=(double x2) {
	((IntervalVector&) (*this))+=x2;
	return *this;
}

inline Tube& Tube::operator+=(const Interval& x2) {
	for (int i=0; i<size();i++) (*this)[i] += x2;
	return *this;
}

inline Tube& Tube::operator+=(const Tube& x2) {
	__assert_tube_time_domain__(*this,x2);
	((IntervalVector&) (*this))+=x2;
	return *this;
}

inline Tube& Tube::operator-=(double x2) {
	((IntervalVector&) (*this))-=x2;
	return *this;
}

inline Tube& Tube::operator-=(const Interval& x2) {
	for (int i=0; i<size();i++) (*this)[i] -= x2;
	return *this;
}

inline Tube& Tube::operator-=(const Tube& x2){
	__assert_tube_time_domain__(*this,x2);
	((IntervalVector&) (*this))-=x2;
	return *this;
}

inline Tube& Tube::operator*=(double x2){
	((IntervalVector&) (*this))*=x2;
	return *this;
}

inline Tube& Tube::operator*=(const Interval& x2){
	((IntervalVector&) (*this))*=x2;
	return *this;
}

inline Tube& Tube::operator*=(const Tube& x2){
	__assert_tube_time_domain__(*this,x2);
	return (*this)=hadamard_product(*this,x2);
}

inline Tube& Tube::operator/=(double x2){
	((IntervalVector&) (*this))*=1/x2;
	return *this;
}

inline Tube& Tube::operator/=(const Interval& x2){
	((IntervalVector&) (*this))*=1/x2;
	return *this;
}

inline Tube& Tube::operator/=(const Tube& x2) {
	__assert_tube_time_domain__(*this,x2);
	for (int i = 0; i < size(); i++) {
		((IntervalVector&) (*this))[i] /= x2[i];
	}
	return *this;
}

inline Tube operator-(const Tube& x) {
	return Tube(x.get_t0(),x.get_tF(),x.get_delta_t(),-x);
}

inline Tube operator+(const Tube& x1, const Tube& x2){
	return Tube(x1)+=x2;
}

inline Tube operator+(const Tube& x1, double x2) {
	return  Tube(x1)+=x2;
}

inline Tube operator+(double x1, const Tube& x2) {
	return Tube(x2)+=x1;
}

inline Tube operator+(const Tube& x1, const Interval& x2) {
	return Tube(x1)+=x2;
}

inline Tube operator+(const Interval& x1, const Tube& x2) {
	return Tube(x2)+=x1;
}

inline Tube operator-(const Tube& x1, const Tube& x2) {
	return Tube(x1)-=x2;
}

inline Tube operator-(const Tube& x1, double x2) {
	return  Tube(x1)-=x2;
}

inline Tube operator-(double x1, const Tube& x2) {
	return Tube(x2)-=x1;
}

inline Tube operator-(const Tube& x1, const Interval& x2) {
	return Tube(x1)-=x2;
}

inline Tube operator-(const Interval& x1, const Tube& x2) {
	return Tube(x2)-=x1;
}

inline Tube operator*(const Tube& x1, const Tube& x2) {
	return Tube(x1)*=x2;
}

inline Tube operator*(const Tube& x1, double x2) {
	return  Tube(x1)*=x2;
}

inline Tube operator*(double x1, const Tube& x2) {
	return Tube(x2)*=x1;
}

inline Tube operator*(const Tube& x1, const Interval& x2) {
	return Tube(x1)*=x2;
}

inline Tube operator*(const Interval& x1, const Tube& x2) {
	return Tube(x2)*=x1;
}

inline Tube operator/(const Tube& x1, const Tube& x2) {
	return Tube(x1)/=x2;
}

inline Tube operator/(const Tube& x1, double x2) {
	return  Tube(x1)/=x2;
}

inline Tube operator/(double x1, const Tube& x2) {
	return Tube(x2)/=x1;
}

inline Tube operator/(const Tube& x1, const Interval& x2) {
	return Tube(x1)/=x2;
}

inline Tube operator/(const Interval& x1, const Tube& x2) {
	return Tube(x2)/=x1;
}


/* ======================== pure scalar functions ===================== */

#define func_unary_tube(f) \
		IntervalVector vec(x.size()); \
		for (int i=0;i<x.size();i++) {vec[i]=f(x[i]);} \
		return Tube(x.get_t0(),x.get_tF(),x.get_delta_t(),vec);

#define func_binary_tube(f) \
		IntervalVector vec(x.size()); \
		for (int i=0;i<x.size();i++) {vec[i]=f(x[i],p);} \
		return Tube(x.get_t0(),x.get_tF(),x.get_delta_t(),vec);

inline Tube abs(const Tube& x)           { func_unary_tube(abs) }
inline Tube sqr(const Tube& x)           { func_unary_tube(sqr) }
inline Tube sqrt(const Tube& x)          { func_unary_tube(sqrt)}
inline Tube pow(const Tube& x, int p)    { func_binary_tube(pow) }
inline Tube pow(const Tube& x, double p) { func_binary_tube(pow) }
inline Tube pow(const Tube &x, const Interval &p) { func_binary_tube(pow) }
inline Tube root(const Tube& x, int p)   { func_binary_tube(root) }
inline Tube exp(const Tube& x)           { func_unary_tube(exp) }
inline Tube log(const Tube& x)           { func_unary_tube(log) }
inline Tube cos(const Tube& x)           { func_unary_tube(cos) }
inline Tube sin(const Tube& x)           { func_unary_tube(sin) }
inline Tube tan(const Tube& x)           { func_unary_tube(tan) }
inline Tube acos(const Tube& x)          { func_unary_tube(acos) }
inline Tube asin(const Tube& x)          { func_unary_tube(asin) }
inline Tube atan(const Tube& x)          { func_unary_tube(atan) }
inline Tube cosh(const Tube& x)          { func_unary_tube(cosh) }
inline Tube sinh(const Tube& x)          { func_unary_tube(sinh) }
inline Tube tanh(const Tube& x)          { func_unary_tube(tanh) }
inline Tube acosh(const Tube& x)         { func_unary_tube(acosh) }
inline Tube asinh(const Tube& x)         { func_unary_tube(asinh) }
inline Tube atanh(const Tube& x)         { func_unary_tube(atanh) }


inline Tube atan2(const Tube& x, const Tube& y) {
	IntervalVector vec(x.size());
	for (int i=0;i<x.size();i++) {
		vec[i]=atan2(x[i],y[i]);
	}
	return Tube(x.get_t0(),x.get_tF(),x.get_delta_t(),vec);
}

} // end namespace ibex

#endif /* _IBEX_TUBE_H_ */
