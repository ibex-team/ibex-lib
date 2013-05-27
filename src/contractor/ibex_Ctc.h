/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CONTRACTOR_H__
#define __IBEX_CONTRACTOR_H__

#include "ibex_IntervalVector.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_BoolMask.h"
#include "ibex_Cell.h"

namespace ibex {

/**
 * \defgroup contractor Contractors
 */

/**
 * \ingroup contractor
 * \brief Contractor interface.
 */
class Ctc {

public:
	/**
	 * \brief Build a contractor for n-dimensional boxes
	 */
	Ctc(int n);

	/**
	 * \brief Contraction.
	 */
	virtual void contract(IntervalVector& box)=0;

	/**
	 * \brief Delete *this.
	 */
	virtual ~Ctc();

	/**
	 * \brief Contraction with specified impact.
	 *
	 * Information on the impact allows incremental contraction.
	 * The \a impact specifies the variables that have been
	 * modified since the last call to this contractor.
	 * By default, this function calls contract(box).
	 *
	 * \see #contract(IntervalVector&).
	 */
	void contract(IntervalVector& box, const BoolMask& impact);

	/**
	 * \brief Contraction with specified impact and output flags.
	 *
	 * \see #contract(IntervalVector&, const BoolMask&).
	 * \see #flags
	 */
	void contract(IntervalVector& box, const BoolMask& impact, BoolMask& flags);

	/**
	 * \brief The number of variables this contractor works with.
	 */
	const int nb_var;

	/**
	 * \brief The input variables
	 */
	BoolMask input;

	/**
	 * \brief The output variables
	 */
	BoolMask output;

	/**
	 * \brief Output flag numbers
	 *
	 * After a call to #contract(IntervalVector& box, const BoolMask& impact, BoolMask& flags):
	 * <ul>
	 * <li> if (flags[FIXPOINT]==true) then the fixpoint was reached by this contractor on the
	 * box which means: \f[C(C(box)) = C(box)\f].
	 * <li> if (flags[INACTIVE]==true) then the fixpoint was reached and this contractor cannot
	 * contract any subbox: \f[\forall [x]\subseteq {\tt box}, \quad C([x])=[x].\f].
	 * </ul>
	 */
	enum {FIXPOINT, INACTIVE, NB_OUTPUT_FLAGS};

protected:
	/**
	 * \brief Return the current impact (NULL pointer if none).
	 *
	 * \return if NULL, it does not mean that there is no impact but that the information on
	 * the impact is not provided.
	 */
	const BoolMask* impact();

	/**
	 * Set an output flag.
	 */
	void set_flag(unsigned int);

private:
	const BoolMask* _impact;
	BoolMask* _output_flags;
};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline const BoolMask* Ctc::impact() {
	return _impact;
}

inline void Ctc::set_flag(unsigned int f) {
	assert(f<NB_OUTPUT_FLAGS);
	if (_output_flags) _output_flags[f]=true;
}

} // namespace ibex

#endif // __IBEX_CONTRACTOR_H__
