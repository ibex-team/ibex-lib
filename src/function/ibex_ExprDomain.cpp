/*
 * ExprData.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: gilles
 */

#include "ibex_Function.h"
#include "ibex_ExprDomain.h"

namespace ibex {

Domain* ExprDomainFactory::init(const ExprIndex& e, Domain& d_expr) {
	switch (e.expr.type()) {
	case Dim::SCALAR:
		return new Domain(d_expr.i());
		break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:
		return new Domain(d_expr.v()[e.index]);
		break;
	case Dim::MATRIX:
		return new Domain(d_expr.m()[e.index],true);
		break;
	default: // Dim::MATRIX_ARRAY:
		return new Domain(d_expr.ma()[e.index]);
		break;
	}
}

Domain* ExprDomainFactory::init(const ExprLeaf& e) {
	return new Domain(e.dim);
}

Domain* ExprDomainFactory::init(const ExprNAryOp& e, Array<Domain>& args_deco) {
	return new Domain(e.dim);
}

Domain* ExprDomainFactory::init(const ExprBinaryOp& e, Domain& left_deco, Domain& right_deco) {
	return new Domain(e.dim);
}

Domain* ExprDomainFactory::init(const ExprUnaryOp& e, Domain& expr_deco) {
	return new Domain(e.dim);
}

Domain* ExprDomainFactory::init(const ExprTrans& e, Domain& expr_deco) {

	if (e.dim.is_vector()) {
		// share references
		return new Domain(expr_deco, true);
	} else {
		// TODO: seems impossible to have references
		// in case of matrices...
		return new Domain(e.dim);
	}
}

/* we could also be more efficient by making symbol
 * direct references to the arguments' domain.
 * However if the same function (hence the same decoration) is
 * used at different places, the references would have to be set
 * dynamically to the actual arguments domains, which is not very clean
 * (would probably require a "set_reference" function in the class Domain).
 * The advantage:
 * - there would be no "args_doms" field in BasicApplyLabel
 * - there would be no copy in fwd_apply and bwd_apply.
 * The last point is crucial in case of symbols that are IntervalMatrixArray
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

