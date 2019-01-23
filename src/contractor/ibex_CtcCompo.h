//============================================================================
//                                  I B E X                                   
// File        : Composition of contractors
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#ifndef __IBEX_CTC_COMPO_H__
#define __IBEX_CTC_COMPO_H__

#include "ibex_Ctc.h"
#include "ibex_Array.h"

namespace ibex {

/** \ingroup contractor
 * \brief Composition of contractors
 *
 * For a box [x] the composition of {c_0,...c_n} performs
 * c_n(...(c_1(c_0([x])))).
 */
class CtcCompo : public Ctc {
public:
	/**
	 * \brief build a composition
	 *
	 * If incremental is true, manages the impact.
	 */
	CtcCompo(const Array<Ctc>& list, bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief build the composition of c1 and c2.
	 *
	 * If incremental is true, manages the impact.
	 */
	CtcCompo(Ctc& c1, Ctc& c2, bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief Create the composition of 3 contractors
	 */
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief Create the composition of 4 contractors
	 */
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief Create the composition of 5 contractors
	 */
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief Create the composition of 6 contractors
	 */
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19,  bool incremental=false, double ratio=default_ratio);
	CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, Ctc& c7, Ctc& c8, Ctc& c9, Ctc& c10, Ctc& c11, Ctc& c12, Ctc& c13, Ctc& c14, Ctc& c15, Ctc& c16, Ctc& c17, Ctc& c18, Ctc& c19, Ctc& c20,  bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief Delete *this.
	 */
	~CtcCompo();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add sub-contractors properties to the map
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/** The list of sub-contractors */
	Array<Ctc> list;

	/** Incremental mode? */
	bool incremental;

	/** Ratio used in incremental mode */
	double ratio;

	/** Default ratio used in incremental mode, set to 0.1. */
	static constexpr double default_ratio = 0.1;

protected:
	void init_impacts();

	BitSet *impacts;
};

} // end namespace ibex
#endif // __IBEX_CTC_COMPO_H__
