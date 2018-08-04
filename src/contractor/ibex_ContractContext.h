//============================================================================
//                                  I B E X
// File        : ibex_ContractContext.h
// Author      : Gilles Chabert
// License     : See the LICENSE file
// Created     : May 29, 2018
//============================================================================

#ifndef __IBEX_CONTRACT_CONTEXT_H__
#define __IBEX_CONTRACT_CONTEXT_H__

#include "ibex_Map.h"
#include "ibex_BoxProperties.h"
#include "ibex_BitSet.h"

namespace ibex {

/**
 * \ingroup ctc
 * \brief Contraction context.
 *
 * Allows to exchange more information with a contractor
 * than just a simple box.
 */
class ContractContext {
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
	 * \brief Create a new and empty context.
	 */
	ContractContext();

	/*
	 * \brief Set the contractor "input" impact
	 *
	 * Information on the impact allows incremental contraction.
	 * The \a impact specifies the variables that have been
	 * modified since the last call to this contractor.
	 */
	void set_impact(BitSet* impact);

	/*
	 * \brief Set the contractor "input" impact
	 */
	void set_impact(const BitSet* impact);

	/*
	 * \brief Set the contractor output flags.
	 */
	void set_output_flags(BitSet* flags);

	/*
	 * \brief Set the box properties before contraction
	 */
	void set_properties(BoxProperties* data);

	/**
	 * \brief Return the current impact (NULL pointer if none).
	 *
	 * \return if NULL, it does not mean that there is no impact but that the information on
	 * the impact is not provided.
	 */
	BitSet* impact();

	/**
	 * \brief Return the current outuput flags (NULL pointer if none).
	 */
	BitSet* output_flags();

	/**
	 * \brief Return the current properties of the box (NULL pointer if none).
	 */
	BoxProperties* prop();

	/**
	 * Set an output flag.
	 */
	void set_flag(unsigned int);

private:
	BitSet* _impact;
	BitSet* _output_flags;
	BoxProperties* _data;
};

/*============================================================================
 	 	 	 	 	 	 	 inline implementation
 ============================================================================*/

inline ContractContext::ContractContext() : _impact(NULL), _output_flags(NULL), _data(NULL) { }

inline void ContractContext::set_impact(BitSet* impact) { _impact = impact; }

inline void ContractContext::set_impact(const BitSet* impact) { _impact = (BitSet*) impact; }

inline void ContractContext::set_output_flags(BitSet* flags) { _output_flags = flags; }

inline void ContractContext::set_properties(BoxProperties* data) { _data = data; }

inline BitSet* ContractContext::impact() { return _impact; }

inline BitSet* ContractContext::output_flags() { return _output_flags; }

inline BoxProperties* ContractContext::prop() { return _data; }

inline void ContractContext::set_flag(unsigned int f) {
	assert(f<ContractContext::NB_OUTPUT_FLAGS);
	if (output_flags())
		output_flags()->add(f);
}

} /* namespace ibex */

#endif /* __IBEX_CONTRACT_CONTEXT_H__ */
