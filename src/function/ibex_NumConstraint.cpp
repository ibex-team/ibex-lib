//============================================================================
//                                  I B E X                                   
// File        : ibex_NumConstraint.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 24, 2013
// Last Update : Feb 24, 2013
//============================================================================

#include "ibex_NumConstraint.h"

using std::pair;

namespace ibex {

#define RETURN(a,b) return pair<const ExprNode*, const Interval*>(a,b)

pair<const ExprNode*, const Interval*> NumConstraint::is_thick_equality() const {

	// an inequality g(x)<=[a,b] is not considered as
	// a thick equality, although it could...
	if (op!=IBEX_EQ) RETURN(NULL,NULL);

	const ExprSub* sub=dynamic_cast<const ExprSub* >(&f.expr());

	if (sub) {

		const ExprConstant* cst=dynamic_cast<const ExprConstant* >(&sub->right);

		if (cst) {
			if (cst->dim.is_scalar())
				RETURN(&sub->left, &cst->get_value());
		} else {

			cst=dynamic_cast<const ExprConstant* >(&sub->left);

			if (cst) {
				if (cst->dim.is_scalar())
					RETURN(&sub->right, &cst->get_value());
			}
		}
	}

	RETURN(NULL,NULL);
}

} // end namespace
