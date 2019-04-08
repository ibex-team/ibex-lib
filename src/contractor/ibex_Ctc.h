/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * Last update : Jul 06, 2018
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CONTRACTOR_H__
#define __IBEX_CONTRACTOR_H__

#include "ibex_Map.h"
#include "ibex_IntervalVector.h"
#include "ibex_BitSet.h"
#include "ibex_Array.h"
#include "ibex_ContractContext.h"
#include "ibex_Set.h"

namespace ibex {

/**
 * \defgroup contractor Contractors
 */

/**
 * \ingroup contractor
 * \brief Contractor interface.
 */
class Ctc {

protected:

	/**
	 * \brief Build a contractor for n-dimensional boxes
	 */
	Ctc(int nb_var);

	/**
	 * \brief Build a contractor for (size of the contractor inside l)-dimensional boxes
	 */
	Ctc(const Array<Ctc>& l);



public:
	/**
	 * \brief Output flag numbers
	 *
	 * After a call to #contract(IntervalVector& box, const BoolMask& impact, BoolMask& flags):
	 * <ul>
	 * <li> if (flags[FIXPOINT]==true) then the fixpoint is reached by this contractor on the
	 * box which means: \f[C(C(box)) = C(box)\f]. Note that the box may have been modified
	 * by the contractor and the fixpoint property is valid for the resulting box (after
	 * contraction).
	 *
	 * <li> if (flags[INACTIVE]==true) then the fixpoint was reached and this contractor cannot
	 * contract any subbox: \f[\forall [x]\subseteq {\tt box}, \quad C([x])=[x].\f]. Note that
	 * this property is valid for the box *before* contraction. Even if the contractor turns
	 * to be inactive on the resulting box (typically, in the case of an empty set) this flag
	 * is not set.
	 * </ul>
	 */
	enum {FIXPOINT, INACTIVE, NB_OUTPUT_FLAGS};

	/**
	 * \brief Delete *this.
	 */
	virtual ~Ctc();

	/**
	 * \brief Contraction.
	 */
	virtual void contract(IntervalVector& box)=0;

	/*
	 * Contract a box with a context.
	 * By default, this function calls contract(box).
	 *
	 * Can be overridden.
	 *
	 * \see #contract(IntervalVector&).
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Contraction with specified impact.
	 *
	 * \deprecated Use contract(IntervalVector& box, ContractContext& context) instead.
	 *
	 * Information on the impact allows incremental contraction.
	 * The \a impact specifies the variables that have been
	 * modified since the last call to this contractor.
	 * By default, this function calls contract(box).
	 *
	 * \see #contract(IntervalVector&).
	 */
	void contract(IntervalVector& box, const BitSet& impact);

	/**
	 * \brief Contraction with specified impact and output flags.
	 *
	 * \deprecated Use contract(IntervalVector& box, ContractContext& context) instead.
	 *
	 * \see #contract(IntervalVector&, const BoolMask&).
	 * \see #flags
	 */
	void contract(IntervalVector& box, const BitSet& impact, BitSet& flags);

	/**
	 * \brief Contract a set with this contractor.
	 *
	 * \param eps - The contractor is applied recursively on the set. This parameter
	 *              is a precision for controlling the recursivity.
	 */
	void contract(Set& set, double eps);

	/**
	 * \brief Add properties required by this contractor.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

	/**
	 * \brief The number of variables this contractor works with.
	 */
	const int nb_var;

	/**
	 * \brief The input variables (NULL pointer means "unspecified")
	 */
	BitSet* input;

	/**
	 * \brief The output variables NULL pointer means "unspecified")
	 */
	BitSet* output;

protected:
	/**
	 * \brief Check if the size of all the contractor of the list is the same.
	 */
	static bool check_nb_var_ctc_list (const Array<Ctc>& l);
};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline Ctc::Ctc(int n) : nb_var(n), input(NULL), output(NULL) { }

inline Ctc::Ctc(const Array<Ctc>& l) : nb_var(l[0].nb_var), input(NULL), output(NULL) { }

inline Ctc::~Ctc() { }

inline void Ctc::contract(IntervalVector& box, ContractContext& context) {
	contract(box);
	// TODO: the input impact of update is the "output" impact
	// of the previous call to contract. Such a field does not
	// exist yet in Context --> set temporarily to "all".
	context.prop.update(BoxEvent(box,BoxEvent::CONTRACT));
}

inline void Ctc::contract(IntervalVector& box, const BitSet& impact) {
	ContractContext context(box);
	context.impact = impact;
	contract(box,context);
}

inline void Ctc::contract(IntervalVector& box, const BitSet& impact, BitSet& flags) {
	ContractContext context(box);
	context.impact = impact;
	contract(box,context);
	flags = context.output_flags;
}

inline void Ctc::add_property(const IntervalVector&, BoxProperties&) {

}

} // namespace ibex

#endif // __IBEX_CONTRACTOR_H__
