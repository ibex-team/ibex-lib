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
#include "ibex_BitSet.h"
#include "ibex_Array.h"
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

public:


	/**
	 * \brief Build a contractor for n-dimensional boxes
	 */
	Ctc(int nb_var);

	/**
	 * \brief Build a contractor for (size of the contractor inside l)-dimensional boxes
	 */
	Ctc(const Array<Ctc>& l);

	/**
	 * \brief Contraction.
	 */
	virtual void contract(IntervalVector& box)=0;

	/**
	 * \brief Contract a set with this contractor.
	 *
	 * \param eps - The contractor is applied recursively on the set. This parameter
	 *              is a precision for controlling the recursivity.
	 */
	void contract(Set& set, double eps);

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
	void contract(IntervalVector& box, const BitSet& impact);

	/**
	 * \brief Contraction with specified impact and output flags.
	 *
	 * \see #contract(IntervalVector&, const BoolMask&).
	 * \see #flags
	 */
	void contract(IntervalVector& box, const BitSet& impact, BitSet& flags);

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


protected:

	/**
	 * \brief Return the current impact (NULL pointer if none).
	 *
	 * \return if NULL, it does not mean that there is no impact but that the information on
	 * the impact is not provided.
	 */
	const BitSet* impact();

	/**
	 * Set an output flag.
	 */
	void set_flag(unsigned int);

protected:
	/**
	 * \brief Check if the size of all the contractor of the list is the same.
	 */
	static bool check_nb_var_ctc_list (const Array<Ctc>& l);

private:
	const BitSet* _impact;
	BitSet* _output_flags;


};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/



inline Ctc::Ctc(int n) : nb_var(n), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) { }

inline Ctc::Ctc(const Array<Ctc>& l) : nb_var(l[0].nb_var), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) { }

inline Ctc::~Ctc() { }

inline const BitSet* Ctc::impact() {
	return _impact;
}

inline void Ctc::set_flag(unsigned int f) {
	assert(f<NB_OUTPUT_FLAGS);
	if (_output_flags) _output_flags->add(f);
}


} // namespace ibex

#endif // __IBEX_CONTRACTOR_H__
