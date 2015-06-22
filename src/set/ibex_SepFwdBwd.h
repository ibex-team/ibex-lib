//============================================================================
//                                  I B E X
// File        : Separator for constraint "f(x) op 0" or "f(x) in y"
// Authors     : Gilles Chabert, Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 01, 2014
// Last Update : Mar 26, 2015
//============================================================================

#ifndef __IBEX_SEP_FWD_BWD_H__
#define __IBEX_SEP_FWD_BWD_H__

#include "ibex_SepCtcPair.h"
#include "ibex_Function.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcNotIn.h"

namespace ibex {

/**
 * \ingroup set
 *
 * \brief Forward-Backward Separator
 *
 *  This separator applies inner and outer forward-backward
 *  contractors to separate a box w.r.t. a constraint
 *  f(x) in [y].
 *
 *  \see ibex::CtcFwdBwd.
 *
 */
class SepFwdBwd : public SepCtcPair {
public:
    /**
     * \brief Build the separator from a constraint "f op 0".
     *
     * Contractors for "f op 0" and "f !op 0" are built internally.
     *
     * \warning The constraint f = 0 cannot be used.
     *
     * \param f  - Function to be used
     * \param op - Operator op in "f op 0"
     */
    SepFwdBwd(Function &f, CmpOp op);

    /**
     * \brief Build the separator for a scalar-valued function
     *
     * \param f - Function to be used
     * \param y - Interval
     */
    SepFwdBwd(Function &f, const Interval &y);

    /**
     * \brief Build the separator for a vector-valued function
     *
     * \param f - Function to be used
     * \param y - Interval vector
     */
    SepFwdBwd(Function &f, const IntervalVector &y);

    /**
     * \brief Build the separator for a matrix-valued function
     *
      * \param f - Function to be used
     * \param y - Interval matrix
     */
    SepFwdBwd(Function &f, const IntervalMatrix& y);

    /**
     * \brief Build the separator for a system
     */
    SepFwdBwd(const System& sys);

    /**
     * \brief Build the separator from a constraint
     */
    SepFwdBwd(NumConstraint& c);

    /**
     *  Destructor
     */
    ~SepFwdBwd();

};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline SepFwdBwd::SepFwdBwd(Function &f, const Interval &y): SepCtcPair(*new CtcNotIn(f,y), *new CtcFwdBwd(f,y)) { }

inline SepFwdBwd::SepFwdBwd(Function &f, const IntervalVector &y): SepCtcPair(*new CtcNotIn(f,y), *new CtcFwdBwd(f,y)) { }

inline SepFwdBwd::SepFwdBwd(Function &f, const IntervalMatrix &y): SepCtcPair(*new CtcNotIn(f,y), *new CtcFwdBwd(f,y)) { }

inline SepFwdBwd::SepFwdBwd(NumConstraint& c) : SepCtcPair(*new CtcFwdBwd(c.f,!c.op), *new CtcFwdBwd(c)) { }

inline SepFwdBwd::~SepFwdBwd() {
	delete &ctc_out;
	delete &ctc_in;
}

} // end namespace ibex

#endif // __IBEX_SEP_FWD_BWD_H__
