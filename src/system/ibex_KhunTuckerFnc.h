/*
 * ibex_FritzJohnFnc.h
 *
 *  Created on: Apr 26, 2017
 *      Author: gilles
 */

#ifndef __IBEX_KHUN_TUCKER_FNC_H__
#define __IBEX_KHUN_TUCKER_FNC_H__

#include "ibex_System.h"
#include "ibex_EntailedCtr.h"

namespace ibex {

/**
 * \ingroup system
 */

//class FritzJohnFnc;

/*
class FritzJohnFncFactory {
public:

	//
	// \warning You should not modify sys.box once this constructor has been called.
	//
	FritzJohnFncFactory(const System& sys);

	FritzJohnFnc build(const IntervalVector& box, const EntailedCtr& entailed);

	~FritzJohnFncFactory();

	const System& sys;

private:
	Function* df;
	Function** dg;

};*/


class KhunTuckerFnc : public Fnc {
public:

	/**
	 * \param dg - NULL if unconstrained problem.
	 *
	 * \param df - Symbolic gradient of the objective
	 *
	 */
	KhunTuckerFnc(const NormalizedSystem& sys, Function* df, Function** dg, const IntervalVector& current_box, const BitSet& active);

	virtual IntervalVector eval_vector(const IntervalVector& x_lambda) const;

	virtual void jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const;

	/**
	 * Return the matrix where the first column is the gradient of f
	 * and the other columns are the gradients of active constraints
	 */
	IntervalMatrix gradients(const IntervalVector& x) const;

	IntervalVector multiplier_domain() const;

	/**
	 * Total number of multipliers
	 */
	const int nb_mult;

//private:
	int n; // number of original variables
	const NormalizedSystem& sys;
	Function* df;
	Function** dg;

	// TODO: put these bitsets in a backtrackable structure

	BitSet eq;
	BitSet ineq;
	BitSet bound_left;
	BitSet bound_right;
};


} /* namespace ibex */

#endif /* IBEX_FRITZJOHNFNC_H_ */
