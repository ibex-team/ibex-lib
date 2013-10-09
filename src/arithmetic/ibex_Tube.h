/* ============================================================================
 * I B E X - Tube definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 7, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_TUBE_H__
#define __IBEX_TUBE_H__

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
class Tube {
private:

	double 			_t0;
	double 			_tf;
	double 			_deltaT;
	IntervalVector 	_vec;	   // vector of elements

public:

	/**
	 * \brief Create [x; ....; x]
	 *
	 * Create a Tube of dimension \a d with
	 * all the components initialized to \a x.
	 */
	Tube(double t0, double tf, double pas, const Interval x= Interval());
	Tube(double t0, double tf, double pas, const IntervalVector x);
	Tube(double t0, double tf, double pas, double  bounds[][2]);
	Tube(double t0, double tf, double pas, const Vector& x);



	double getT0() const;
	double getTf() const;
	double getDeltaT() const;


	/**
	 * \brief Create [empty; ...; empty]
	 *
	 * Create an empty Tube of dimension \a n
	 * (all the components being empty Intervals)
	 *
	 * \pre n>0
	 */
	static Tube empty(double t0, double tf, double pas);

	/**
	 * \brief Delete this vector
	 */
	virtual ~Tube() {};

	/**
	 * \brief Return the Interval corresponding to the time \a ti
	 *
	 * A return a const reference to the
	 * ti^th time (ti starts from t0 to tf)
	 */
	const Interval& t(double ti) const;
	Interval& t(double ti);


	/**
	 * \brief Return the Interval corresponding to the time \a ti
	 *
	 * A return a const reference to the
	 * i^th time (ti starts from t0 to tf)
	 */
	const Interval& operator[](int i) const;
	Interval& operator[](int i);

	/**
	 * \brief Set this Tube to the empty Tube
	 *
	 * The dimension remains the same.
	 */
	void set_empty();

	/**
	 * \brief Set all the elements to 0 (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void clear();

	/**
	 * \brief Set all the elements to x (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const IntervalVector& x);
	void init(const Interval& x);

	/**
	 * \brief Resize this Tube.
	 *
	 */
	void resize(double new_deltaT);

	/**
	 * \brief Return a subTube.
	 *
	 * \pre (*this) must not be empty and (t0> _t0) and (tf<_tf)
	 * \return [ (*this)[t0]; ...; (*this)[tf] ].
	 */
	Tube subTube(double t0, double tf) const;

	/**
	 * \brief Assign this Tube to x.
	 *
	 * \pre Dimensions of this and x must match.
	 * \note Emptiness is overridden.
	 */
	Tube& operator=(const Tube& x);
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

	/**
	 * \brief The dimension (number of components)
	 */
	int size() const;

	/**
	 * \brief Return the lower bound vector
	 * \pre (*this) must be nonempty
	 */
	Vector lb() const;

	/**
	 * \brief Return the upper bound vector
	 * \pre (*this) must be nonempty
	 */
	Vector ub() const;

	/**
	 * \brief Return the midpoint
	 * \pre (*this) must be nonempty
	 */
	Vector mid() const;

	/**
	 * \brief Return the mignitude vector.
	 * \pre (*this) must be nonempty
	 */
	Vector mig() const;

	/**
	 * \brief Return the magnitude vector.
	 * \pre (*this) must be nonempty
	 */
	Vector mag() const;

	/**
	 * \brief Return true iff this Tube is empty
	 */
	bool is_empty() const;

	/**
	 * \brief True iff this interval vector contains \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::contains(double) const.
	 */
	bool contains(const Vector& x) const;

	/**
	 * \brief true iff this interval vector contains an infinite bound.
	 *
	 * \note An empty interval vector is always bounded.
	 */
	bool is_unbounded() const;

	/**
	 * \brief True iff this interval vector is a subset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if this interval vector is empty.

	 * \sa #ibex::Interval::is_subset(const Interval&) const.
	 */
	bool is_subset(const Tube& x) const;

	/**
	 * \brief True iff this interval vector is inside the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.
	 *
	 * \note return true if this interval vector is empty and \a x not.
	 *
	 * \sa #ibex::Interval::is_strict_subset(const Interval&) const.
	 */
	bool is_strict_subset(const Tube& x) const;

	/**
	 * \brief True iff this interval vector is a superset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if \a x is empty.

	 * \sa #ibex::Interval::is_superset(const Interval&) const.
	 */
	bool is_superset(const Tube& x) const;

	/**
	 * \brief True iff \a x is inside the interior of (*this).
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.
	 *
	 * \note return true if x is empty and not (*this).
	 *
	 * \sa #ibex::Interval::is_strict_superset(const Interval&) const.
	 */
	bool is_strict_superset(const Tube& x) const;


	/**
	 * \brief Vector of radii.
	 */
	Vector rad() const;

	/**
	 * \brief Return the vector of diameters.
	 */
	Vector diam() const;

	/**
	 * \brief Return the index of a component with minimal/maximal diameter.
	 *
	 *  \param min true => minimal diameter
	 *  \throws InvalidTubeOp if the Tube is empty.
	 */
	int extr_diam_index(bool min) const;


	/**
	 * \brief Return the maximal diameter among all the components.
	 *
	 *  \throws InvalidTubeOp if the Tube is empty.
	 */
	double max_diam() const;

	/**
	 * \brief Return the minimal diameter among all the components.
	 *
	 * \throws InvalidTubeOp if the Tube is empty.
	 */
	double min_diam() const;

	/**
	 * \brief Return the volume of this interval vector.
	 *
	 * \note Return \c POS_INFINITY if the vector is unbounded and not flat.
	 * \note Return 0 if the vector is flat and not unbounded.
	 * \warning If the interval vector is both flat and unbounded, the result is undefined.
	 * \sa #flat()
	 * \sa #unbounded()
	 */
	double volume() const;

	/**
	 * \brief Bisect the box
	 *
	 * The box is bisected along the dimension \a i
	 * and with a ratio \a ratio. If (*this)[i] is the interval [a,a+d]:
	 * <ul>
	 * <li> The first box of the result is (*this)[0]x...x(*this)[i-1]x[a+ratio*d]x...
	 * <li> The second box is (*this)[0]x...x(*this)[i-1]x[a+ratio*d,a+d]x...
	 * </ul>
	 * Default value for the ratio is 0.5.
	 * \pre 0<ratio<1
	 */
	std::pair<Tube,Tube> bisect(int i, double ratio=0.5) const;

	/**
	 * \brief Return a random vector inside *this.
	 *
	 * \pre (*this) must be nonempty.
	 */
	Vector random() const;

	/**
	 * \brief (*this)+=x2.
	 */
	Tube& operator+=(double x2);
	Tube& operator+=(const Interval& x2);
	Tube& operator+=(const Tube& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	Tube& operator-=(double x2);
	Tube& operator-=(const Interval& x2);
	Tube& operator-=(const Tube& x2);

	/**
	 * \brief *this)*=x2
	 */
	Tube& operator*=(double x2);
	Tube& operator*=(const Interval& x2);
	Tube& operator*=(const Tube& x2);

	/**
	 * \brief *this)/=x2
	 */
	Tube& operator/=(double x2);
	Tube& operator/=(const Interval& x2);
	Tube& operator/=(const Tube& x2);

	/**
	 * \brief Contract *this according to the state equation \a f
	 */
	Tube& ctcFwd(const Function &f);
	Tube& ctcBwd(const Function &f);
	Tube& ctcFwdBwd(const Function &f);

	/**
	 *\brief  Contract the tube with a periodic \a t
	 */
	Tube& ctcPeriodic( double t);
	Tube& ctcPeriodic( const Interval &t);


	/**
	 * \brief Contract the tube with symmetric constraint
	 */
	Tube& ctcSym( double pivot);
	Tube& ctcSym( const Interval &pivot);


	/**
	 * \brief Contract the tube with antisymmetric constraint
	 */
	Tube& ctcASym( double pivot);
	Tube& ctcASym( const Interval &pivot);

	/**
	 * \brief Add a delay \a t to the tube *this
	 */
	Tube& ctcDelay( double t);
	Tube& ctcDelay( const Interval &t);

	/**
	 * \brief Scale the time of the tube *this by \a alpha
	 */
	Tube& ctcScale( double alpha);
	Tube& ctcScale( const Interval &alpha);

};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief -x.
 */
Tube operator-(const Tube& x);

/**
 * \brief x1+x2.
 */
Tube operator+(const Tube& x1, const Tube& x2);
Tube operator+(const Tube& x1, double x2);
Tube operator+(double x1, const Tube& x2);
Tube operator+(const Tube& x1, const Interval& x2);
Tube operator+(const Interval& x1, const Tube& x2);


/**
 * \brief x1-x2.
 */
Tube operator-(const Tube& x1, const Tube& x2);
Tube operator-(const Tube& x1, double x2);
Tube operator-(double x1, const Tube& x2);
Tube operator-(const Tube& x1,  const Interval& x2);
Tube operator-(const Interval& x1, const Tube& x2);

/**
 * \brief x1*x2.
 */
Tube operator*(double x1, const Tube& x2);
Tube operator*(double x1, const Tube& x2);
Tube operator*(const Tube& x1, double x2);
Tube operator*(const Interval& x1, const Tube& x2);
Tube operator*(const Tube& x1, const Interval& x2);

/**
 * \brief x1/x2.
 */
Tube operator/(double x1, const Tube& x2);
Tube operator/(double x1, const Tube& x2);
Tube operator/(const Tube& x1, double x2);
Tube operator/(const Interval& x1, const Tube& x2);
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
 * \brief Return the tube based on \a x with a period \a d
 */
Tube periodic( const Tube &x, double t);
Tube periodic( const Tube &x, const Interval &t);


/**
 * \brief Return the tube based on \a x with symmetric axe \a pivot
 */
Tube sym( const Tube &x, double pivot);
Tube sym( const Tube &x, const Interval &pivot);


/**
 * \brief Return the tube based on \a x with asymmetric axe \a pivot
 */
Tube asym( const Tube &x, double pivot);
Tube asym( const Tube &x, const Interval &pivot);

/**
 * \brief Return the tube based on \a x with a delay \a t
 */
Tube delay( const Tube &x, double t);
Tube delay( const Tube &x, const Interval &t);

/**
 * \brief Return the tube based on \a x with scaling the time by \a alpha
 */
Tube scale( const Tube &x, double alpha);
Tube scale( const Tube &x, const Interval &alpha);

/**
 * \brief Return the tube \a f(\a x)
 */
Tube compose(const Function &f, const Tube &x);

/**
 * \brief Display the Tube \a x
 */
std::ostream& operator<<(std::ostream& os, const Tube& x);


/*@}*/

/*============================================ inline implementation ============================================ */

inline void Tube::init(const IntervalVector& x) {
	 _vec = x ;
}

inline void Tube::init(const Interval& x) {
	 _vec.init(x) ;
}

inline double Tube::getT0() const{
	return _t0;
}

inline double Tube::getTf() const{
	return _tf;
}

inline double Tube::getDeltaT() const{
	return _deltaT;
}

inline Tube Tube::empty(double t0, double tf, double deltaT) {
	return Tube(t0,tf,deltaT,Interval::EMPTY_SET);
}

inline void Tube::set_empty() {
	_vec[0]=Interval::EMPTY_SET;
}

inline const Interval& Tube::operator[](int i) const {
	assert(0<=i && i<size());
	return _vec[i];
}

inline Interval& Tube::operator[](int i) {
	assert(0<=i && i<size());
	return _vec[i];
}

inline const Interval& Tube::t(double t) const {
	assert(t>=_t0 && t<_tf);
	return _vec[(int)((t-_t0)/_deltaT)];
}

inline Interval& Tube::t(double t) {
	assert(t>=_t0 && t<_tf);
	return _vec[(int)((t-_t0)/_deltaT)];
}

inline void Tube::clear() {
	init(0);
}

inline Tube Tube::operator&(const Tube& x) const {
	return Tube(*this) &= x;
}

inline Tube Tube::operator|(const Tube& x) const {
	return Tube(*this) |= x;
}

inline int Tube::size() const {
	return _vec.size();
}

inline Vector Tube::lb() const{
	return _vec.lb();
}

inline Vector Tube::ub() const{
	return _vec.ub();
}

inline Vector Tube::mid() const{
	return _vec.mid();
}

inline Vector Tube::mig() const{
	return _vec.mig();
}

inline Vector Tube::mag() const{
	return _vec.mag();
}

inline bool Tube::is_empty() const {
	return _vec[0].is_empty();
}

inline bool Tube::contains(const Vector& x) const{
	return _vec.contains(x);
}

inline bool Tube::is_unbounded() const{
	return _vec.is_unbounded();
}

inline bool Tube::is_subset(const Tube& x) const {
	return _vec.is_subset(x._vec);
}

inline bool Tube::is_strict_subset(const Tube& x) const {
	return _vec.is_strict_subset(x._vec);
}

inline bool Tube::is_superset(const Tube& x) const {
	return x.is_subset(*this);
}

inline bool Tube::is_strict_superset(const Tube& x) const {
	return x.is_strict_subset(*this);
}

inline Vector Tube::rad() const{
	return _vec.rad();
}

inline Vector Tube::diam() const{
	return _vec.diam();
}

inline int Tube::extr_diam_index(bool min) const{
	return _vec.extr_diam_index(min);
}

inline double Tube::max_diam() const {
	return _vec[extr_diam_index(false)].diam();
}

inline double Tube::min_diam() const {
	return _vec[extr_diam_index(true)].diam();
}

inline double Tube::volume() const {
	return _vec.volume();
}

inline Vector Tube::random() const{
	return _vec.random();
}

inline Tube& Tube::operator+=(double x2){
	for (int i=0; i<size();i++) {_vec[i] += x2;}
	return *this;
}
inline Tube& Tube::operator+=(const Interval& x2){
	for (int i=0; i<size();i++) {_vec[i] += x2;}
	return *this;
}
inline Tube& Tube::operator+=(const Tube& x2){
	assert((_t0==x2._t0)&&(_tf==x2._tf));
	_vec += x2._vec;
	return *this;
}

inline Tube& Tube::operator-=(double x2) {
	for (int i=0; i<size();i++) {_vec[i] -= x2;}
	return *this;
}
inline Tube& Tube::operator-=(const Interval& x2) {
	for (int i=0; i<size();i++) {_vec[i] -= x2;}
	return *this;
}
inline Tube& Tube::operator-=(const Tube& x2){
	assert((_t0==x2._t0)&&(_tf==x2._tf));
	_vec -= x2._vec;
	return *this;
}


inline Tube& Tube::operator*=(double x2){
	_vec *= x2;
	return *this;
}
inline Tube& Tube::operator*=(const Interval& x2){
	_vec *= x2;
	return *this;
}
inline Tube& Tube::operator*=(const Tube& x2){
	assert((_t0==x2._t0)&&(_tf==x2._tf));
	for (int i=0; i<size();i++) {_vec[i] *= x2._vec[i];}
	return *this;
}

inline Tube& Tube::operator/=(double x2){
	return *this *=(1/Interval(x2));
}

inline Tube& Tube::operator/=(const Interval& x2){
	return *this *= 1/x2;
}


inline Tube operator-(const Tube& x){
	IntervalVector vec(x.size());
	for (int i=0;i<x.size();i++) {vec[i]=-(x[i]);}
	return Tube(x.getT0(),x.getTf(),x.getDeltaT(),vec);
}


inline Tube operator+(const Tube& x1, const Tube& x2){
	return Tube(x1) += x2;
}
inline Tube operator+(const Tube& x1, double x2){
	return  Tube(x1)+= x2;
}
inline Tube operator+(double x1, const Tube& x2){
	return Tube(x2) += x1;
}
inline Tube operator+(const Tube& x1, const Interval& x2){
	return Tube(x1) += x2;
}
inline Tube operator+(const Interval& x1, const Tube& x2){
	return Tube(x2) += x1;
}


inline Tube operator-(const Tube& x1, const Tube& x2){
	return Tube(x1) -= x2;
}
inline Tube operator-(const Tube& x1, double x2){
	return  Tube(x1)-= x2;
}
inline Tube operator-(double x1, const Tube& x2){
	return Tube(x2) -= x1;
}
inline Tube operator-(const Tube& x1, const Interval& x2){
	return Tube(x1) -= x2;
}
inline Tube operator-(const Interval& x1, const Tube& x2){
	return Tube(x2) -= x1;
}


inline Tube operator*(const Tube& x1, const Tube& x2){
	return Tube(x1) *= x2;
}
inline Tube operator*(const Tube& x1, double x2){
	return  Tube(x1)*= x2;
}
inline Tube operator*(double x1, const Tube& x2){
	return Tube(x2) *= x1;
}
inline Tube operator*(const Tube& x1, const Interval& x2){
	return Tube(x1) *= x2;
}
inline Tube operator*(const Interval& x1, const Tube& x2){
	return Tube(x2) *= x1;
}


inline Tube operator/(const Tube& x1, const Tube& x2){
	return Tube(x1) /= x2;
}
inline Tube operator/(const Tube& x1, double x2){
	return  Tube(x1)/= x2;
}
inline Tube operator/(double x1, const Tube& x2){
	return Tube(x2) /= x1;
}
inline Tube operator/(const Tube& x1, const Interval& x2){
	return Tube(x1) /= x2;
}
inline Tube operator/(const Interval& x1, const Tube& x2){
	return Tube(x2) /= x1;
}



inline Tube periodic( const Tube &x, double t) {
	return Tube(x).ctcPeriodic(t);
}
inline Tube periodic( const Tube &x, const Interval &t) {
	return Tube(x).ctcPeriodic(t);
}

inline Tube sym( const Tube &x, double pivot) {
	return Tube(x).ctcSym(pivot);
}
inline Tube sym( const Tube &x, const Interval &pivot) {
	return Tube(x).ctcSym(pivot);
}



inline Tube asym( const Tube &x, double pivot) {
	return Tube(x).ctcASym(pivot);
}
inline Tube asym( const Tube &x, const Interval &pivot) {
	return Tube(x).ctcASym(pivot);
}


inline Tube delay( const Tube &x, double t){
	return Tube(x).ctcDelay(t);
}
inline Tube delay( const Tube &x, const Interval &t) {
	return Tube(x).ctcDelay(t);
}


inline Tube scale( const Tube &x, double alpha) {
	return Tube(x).ctcScale(alpha);
}
inline Tube scale( const Tube &x, const Interval &alpha) {
	return Tube(x).ctcScale(alpha);
}




/* ======================== pure scalar functions ===================== */

#define func_unary_tube(f) \
		IntervalVector vec(x.size()); \
		for (int i=0;i<x.size();i++) {vec[i]=f(x[i]);} \
		return Tube(x.getT0(),x.getTf(),x.getDeltaT(),vec);

#define func_binary_tube(f) \
		IntervalVector vec(x.size()); \
		for (int i=0;i<x.size();i++) {vec[i]=f(x[i],p);} \
		return Tube(x.getT0(),x.getTf(),x.getDeltaT(),vec);




inline Tube abs(const Tube& x) { func_unary_tube(abs); }
inline Tube sqr(const Tube& x) { func_unary_tube(sqr); }
inline Tube sqrt(const Tube& x) { func_unary_tube(sqrt); }
inline Tube pow(const Tube& x, int p) { func_binary_tube(pow); }
inline Tube pow(const Tube& x, double p) { func_binary_tube(pow); }
inline Tube pow(const Tube &x, const Interval &p) { func_binary_tube(pow); }
inline Tube root(const Tube& x, int p) { func_binary_tube(root); }
inline Tube exp(const Tube& x) { func_unary_tube(exp); }
inline Tube log(const Tube& x) { func_unary_tube(log); }
inline Tube cos(const Tube& x) { func_unary_tube(cos); }
inline Tube sin(const Tube& x) { func_unary_tube(sin); }
inline Tube tan(const Tube& x) { func_unary_tube(tan); }
inline Tube acos(const Tube& x) { func_unary_tube(acos); }
inline Tube asin(const Tube& x) { func_unary_tube(asin); }
inline Tube atan(const Tube& x) { func_unary_tube(atan); }
inline Tube cosh(const Tube& x) { func_unary_tube(cosh); }
inline Tube sinh(const Tube& x) { func_unary_tube(sinh); }
inline Tube tanh(const Tube& x) { func_unary_tube(tanh); }
inline Tube acosh(const Tube& x) { func_unary_tube(acosh); }
inline Tube asinh(const Tube& x) { func_unary_tube(asinh); }
inline Tube atanh(const Tube& x) { func_unary_tube(atanh); }



inline Tube atan2(const Tube& x, const Tube& y) {
	IntervalVector vec(x.size());
	for (int i=0;i<x.size();i++) {vec[i]=atan2(x[i],y[i]);}
	return Tube(x.getT0(),x.getTf(),x.getDeltaT(),vec);
}






}

  // end namespace

#endif /* _IBEX_INTERVAL_VECTOR_H_ */
