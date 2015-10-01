//============================================================================
//                                  I B E X                                   
// File        : ibex_Pdc.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#ifndef __IBEX_PDC_H__
#define __IBEX_PDC_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"

namespace ibex {

/**
 * \defgroup predicate Predicates
 */

/**
 * \ingroup predicate
 * \brief Predicate (function that maps a box to a boolean interval)
 *
 * Root class of all predicates.
 *
 */
class Pdc {
public:

	/**
	 * \brief Build a predicate for n-dimensional boxes
	 */
	Pdc(int nb_var);

	/**
	 * \brief Build with a list of predicates
	 */
	Pdc(const Array<Pdc>& list);


	/**
	 * \brief Apply the predicate to the given box.
	 */
	virtual BoolInterval test(const IntervalVector& box)=0;

	/**
	 * \brief Delete *this.
	 */
	virtual ~Pdc();

	/**
	 * \brief The number of variables this predicate works with.
	 */
	const int nb_var;


protected:

	/**
	 * \brief Check if the size of all the predicate of the list is the same.
	 */
	static bool check_nb_var_pdc_list (const Array<Pdc>& l) {
		int i=1;
		while ( i<l.size() && (l[i].nb_var==l[0].nb_var)) {
			i++;
		}
		return (i==l.size());
	}

};


/*================================== inline implementations ========================================*/

inline Pdc::Pdc(int n) : nb_var(n) { }

inline Pdc::Pdc(const Array<Pdc>& l) : nb_var(l[0].nb_var) { }

inline Pdc::~Pdc() { }


} // end namespace ibex
#endif // __IBEX_PDC_H__
