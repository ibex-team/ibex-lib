/*
 * ExprData.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: gilles
 */

#include "ibex_Function.h"
#include "ibex_ExprDomain.h"

namespace ibex {


/* we could also be more efficient by making symbol
 * direct references to the arguments' domain.
 * However if the same function (hence the same decoration) is
 * used at different places, the references would have to be set
 * dynamically to the actual arguments domains, which is not very clean
 * (would probably require a "set_reference" function in the class Domain).
 * The advantage:
 * - there would be no "args_doms" field in BasicApplyLabel
 * - there would be no copy in fwd_apply and bwd_apply.
 * The last point is crucial in case of symbols that are IntervalMatrix
 * (i.e., that have potentially big domains w.r.t. memory).
 */
//	for (int i=0; i<a.nb_args; i++) {
//		((BasicApplyLabel*) a.deco)->args_doms.set(i,(Domain&) (*a.arg(i).deco));
//	}
//}

/*
BasicApplyLabel::BasicApplyLabel(const Dim& dim, Function& f) :
	// same comment as above. We could make the root of the sub-function f
	// being a reference (set dynamically) to the corresponding node in the
	// DAG of the function.
	//
	Domain(dim), args_doms(f.nb_symbols()), f(f), fevl(f) {

}*/
} // namespace ibex

