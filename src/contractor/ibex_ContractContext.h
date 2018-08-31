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
 *
 * \brief Contraction context.
 *
 * Allows to exchange more information with a contractor
 * than just a simple box.
 *
 * Note: A reference to the box to be contracted is stored in prop.box.
 */
class ContractContext {
public:
	/**
	 * \brief Create a new and empty context.
	 */
	ContractContext(const IntervalVector& box);

	/**
	 * \brief Copy a context.
	 *
	 * The properties of the copy are associated to the same box as c.prop.
	 */
	ContractContext(const ContractContext& c);

	/**
	 * \brief Copy a context for a new (copy of the) box.
	 */
	ContractContext(const IntervalVector& box, const ContractContext& c);

	/*
	 * \brief Contractor "input" impact
	 *
	 * Information on the impact allows incremental contraction.
	 * The \a impact specifies the variables that have been
	 * modified since the last call to this contractor.
	 */
	BitSet impact;

	/**
	 * \brief Contractor "output" flags
	 *
	 * See Ctc's output flags values (enum)
	 */
	BitSet output_flags;

	/**
	 * \brief Properties of the box to be contracted
	 */
	BoxProperties prop;
};

/*============================================================================
 	 	 	 	 	 	 	 inline implementation
 ============================================================================*/

inline ContractContext::ContractContext(const IntervalVector& box) : impact(BitSet::all(box.size())), output_flags(box.size()), prop(box) {

}

inline ContractContext::ContractContext(const ContractContext& c) : ContractContext(c.prop.box, c) {

}

inline ContractContext::ContractContext(const IntervalVector& box, const ContractContext& c) : impact(c.impact), output_flags(c.output_flags), prop(box, c.prop) {

}


} /* namespace ibex */

#endif /* __IBEX_CONTRACT_CONTEXT_H__ */
