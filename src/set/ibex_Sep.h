/* ============================================================================
 * I B E X - Separator interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (France)
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Mines de Nantes (France)
 *               Benoit Desrochers, ENSTA Bretagne (France)
 *
 * Created     : Mar 24, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_SEPARATOR_H__
#define __IBEX_SEPARATOR_H__

#include "ibex_IntervalVector.h"
#include "ibex_Set.h"
#include "ibex_SetInterval.h"

namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Separator interface.
 *
 * A separator is an operator that performs two independent and
 * complementary contractions. The separator is associated with
 * a set (noted S) and the first contraction (called "inner")
 * removes points inside S. The second contraction (called
 * "outer") removes points outside S.
 *
 * In summary:
 * Given a box [x], the separator produces two sub-boxes
 * [x_in] and [x_out] that verify:
 *
 *     ([x] \ [x_in]) \subset S
 *     ([x] \ [x_out]) \cap S = \emptyset
 *
 * For efficiency reasons, the separate(...) function takes only
 * two input-output arguments, x_in and x_out, each containing
 * initially a copy of the box [x].
 *
 * A separator can also be viewed a as pair of contractors.
 * See ibex::SepCtcPair.
 *
 * See L. Jaulin and B. Desrochers (2014). "Introduction to the
 * Algebra of Separators with Application to Path Planning".
 * Engineering Applications of Artificial Intelligence volume 33, pp. 141-147.
 *
 *
 */
class Sep {

public:
	/**
	 * \brief Build a separatot for n-dimensional boxes
	 */
	Sep(int nb_var);

  /**
   * \brief Separate a box in two sub-boxes.
   *
   * \param x_in  - As input: the initial box. As output:
   *                result of the first ("inner") contraction
   * \param x_out - As input: the initial box. As output:
   *                the result of the second ("outer") contraction
   *
   * Precondition: x_in and x_out must be the same boxes.
	 */
  virtual void separate(IntervalVector& x_in, IntervalVector& x_out) = 0;

	/**
	 * \brief Contract a set with this separator.
	 *
	 * If [x] designates the set in argument and [y] the set implicitly represented by this separator,
	 * then [x] will be replaced by :
	 *
	 * { x \cap y, x\in[x] and y\in[y] }.
	 *
	 * Note: In Jaulin's terminology, this operator is the "i-set extension of the intersection".
	 *
	 * \param eps - The separator is applied recursively on the set. This parameter
	 *              is a precision for controlling the recursivity.
	 */
	void contract(Set& set, double eps);

	/**
	 * \brief Contract an i-set with this separator.
	 *
	 * In Jaulin's terminology, this operator is the "intersection of i-sets" (squared symbol)
	 *
	 * If [x] designates the i-set in argument and [y] the i-set implicitly represented by this separator,
	 * then [x] will be replaced by
	 *
	 *  { x, x\in[x] and x\in[y] }.
	 *
	 * \param eps - The separator is applied recursively on the i-set. This parameter
	 *              is a precision for controlling the recursivity.
	 */
	void contract(SetInterval& iset, double eps, BoolInterval status1=YES, BoolInterval status2=NO);

	/**
	 * \brief Delete *this.
	 */
  virtual ~Sep();

   /**
	 * \brief The number of variables this contractor works with.
	 */
	const int nb_var;

  // Get current status of the 1st contraction
  // (Used by SetBisect & SetLeaf)
  BoolInterval status1() const;

  // Get current status of the 2nd contraction
  // (Used by SetBisect & SetLeaf)
  BoolInterval status2() const;

private:

    BoolInterval _status1;
    BoolInterval _status2;
};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline Sep::Sep(int n) : nb_var(n), _status1(YES), _status2(NO) { }

inline Sep::~Sep() { }

inline BoolInterval Sep::status1() const { return _status1; }

inline BoolInterval Sep::status2() const { return _status2; }

} // namespace ibex

#endif // __IBEX_SEPARATOR_H__
