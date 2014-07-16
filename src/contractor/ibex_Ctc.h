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
#include "ibex_Array.h"

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
	 * \brief The input variables (NULL pointer means "unspecified")
	 */
	BoolMask* input;

	/**
	 * \brief The output variables NULL pointer means "unspecified")
	 */
	BoolMask* output;

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

	/**
	 * \brief Check if the size of all the contractor of the list is the same.
	 */
	static bool check_nb_var_ctc_list (const ibex::Array<ibex::Ctc>& l)  {
		int i=1, n=l[0].nb_var;
		while ((l[i].nb_var==l[0].nb_var)&&(i<l.size())) {
			i++;
		}
		return (i==l.size());
	}
};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/


inline Ctc::Ctc(int n) : nb_var(n), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) {
}

inline Ctc::Ctc(const Array<Ctc>& l) : nb_var(l[0].nb_var), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) {
}

inline Ctc::~Ctc() {
}

inline const BoolMask* Ctc::impact() {
	return _impact;
}

inline void Ctc::set_flag(unsigned int f) {
	assert(f<NB_OUTPUT_FLAGS);
	if (_output_flags) (*_output_flags)[f]=true;
}


} // namespace ibex

#endif // __IBEX_CONTRACTOR_H__
