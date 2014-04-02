/* ============================================================================
 * I B E X - Constant Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 31, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_CONSTANT_H__
#define __IBEX_FNC_CONSTANT_H__

#include "ibex_Fnc.h"
#include "ibex_Domain.h"

namespace ibex {

/**
 * \ingroup function
 * \brief Constant function
 */
class FncConstant : public Fnc {
public:

	FncConstant(const Domain& d);

	/** \brief Override */
	virtual Interval eval(const IntervalVector& box) const;

	/** \brief Override */
	virtual IntervalVector eval_vector(const IntervalVector& box) const;

	/** \brief Override */
	virtual IntervalMatrix eval_matrix(const IntervalVector& x) const;

	/** \brief Override */
	virtual void gradient(const IntervalVector& x, IntervalVector& g) const;

	/** \brief Override */
	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J) const;

	/**
	 * The constant
	 */
	Domain cst;

protected:
	/** \brief Override */
	virtual void generate_comp() const;
	/** \brief Override */
	virtual void generate_used_vars() const;
	/** \brief Override */
	virtual void print(std::ostream& os) const;
	/** \brief Override */
	void print_expr(std::ostream& os) const;

	// ========== never understood why we have to do this in c++ =================
	IntervalVector gradient(const IntervalVector& x) const;
	IntervalMatrix jacobian(const IntervalVector& x) const;
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const;
	int nb_used_vars() const;
	int used_var(int i) const;
	// ============================================================================
};


/*================================== inline implementations ========================================*/

inline FncConstant::FncConstant(const Domain& d) : cst(d) {

}

inline Interval FncConstant::eval(const IntervalVector& box) const {
	return cst.i();
}

inline IntervalVector FncConstant::eval_vector(const IntervalVector& box) const {
	return cst.v();
}

inline IntervalMatrix FncConstant::eval_matrix(const IntervalVector& x) const {
	return cst.m();
}

inline void FncConstant::gradient(const IntervalVector& x, IntervalVector& g) const {
	g.clear();
}

inline void FncConstant::jacobian(const IntervalVector& x, IntervalMatrix& J) const {
	J.clear();
}

inline void FncConstant::generate_used_vars() const {
	_nb_used_vars=0;
}

inline void FncConstant::print(std::ostream& os) const {
	os << "x[" << nb_var() << "]->" << cst;
}

inline void FncConstant::print_expr(std::ostream& os) const {
	os << cst;
}

// ========== never understood why we have to do this in c++ =================
inline IntervalVector FncConstant::gradient(const IntervalVector& x) const {
	return Fnc::gradient(x);
}

inline IntervalMatrix FncConstant::jacobian(const IntervalVector& x) const {
	return Fnc::jacobian(x);
}

inline void FncConstant::hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const {
	Fnc::hansen_matrix(x,h);
}

inline int FncConstant::nb_used_vars() const {
	return Fnc::nb_used_vars();
}

inline int FncConstant::used_var(int i) const {
	return Fnc::used_var(i);
}
// ============================================================================

} // end namespace ibex
#endif // __IBEX_FNC_CONSTANT_H__
