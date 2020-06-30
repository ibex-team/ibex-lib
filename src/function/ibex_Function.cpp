/* ============================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Function.h"
#include "ibex_ExprCopy.h"
#include "ibex_Eval.h"
#include "ibex_HC4Revise.h"
#include "ibex_InHC4Revise.h"
#include "ibex_Gradient.h"
#include "ibex_ExprFuncDomain.h"

using namespace std;

namespace ibex {

const System& Function::def_domain() const {
	if (!_def_domain)
		(System*&) _def_domain = ExprFuncDomain(*this).get();
	return *_def_domain;
}

Function::~Function() {

	// note: destructor of Eval requires *this
	if (_eval!=NULL) {
		delete _eval;
		delete _hc4revise;
		delete _grad;
		delete _inhc4revise;
	}

	if (comp!=NULL) {
		/* warning... if there is only one constraint
		 * then comp[0] is the same object as f itself!
		 *
		 * This is not a very consistent choice...
		 */
		if (image_dim()>1) {
			int m=_image_dim.is_vector() ? _image_dim.vec_size() : _image_dim.nb_rows();
			for (int i=0; i<m; i++)
				if (!zero || comp[i]!=zero) delete comp[i];
		}
		if (zero) delete zero;
		delete[] comp;
	}

	if (cf.code!=NULL) {

		cleanup(expr(),false);

		for (int i=0; i<nb_arg(); i++) {
			delete &arg(i);
		}
	}

	if (df!=NULL) delete df;

	if (_def_domain!=NULL) delete _def_domain;

	if (name!=NULL) { // name==NULL if init/build_from_string was never called.
		free((char*) name);
		delete[] __symbol_index;
	}
}

void Function::print(std::ostream& os) const {
	if (name!=NULL) os << name << ":";
	os << "(";
	for (int i=0; i<nb_arg(); i++) {
		const ExprSymbol& x = arg(i);
		os << x;
		if (x.dim.nb_rows()>1) os << '[' << x.dim.nb_rows() << ']';
		if (x.dim.nb_cols()>1) {
			if (x.dim.nb_rows()==1) os << "[1]";
			os << '[' << x.dim.nb_cols() << ']';
		}
		if (i<nb_arg()-1) os << ",";
	}
	os << ")->" << expr();
}

const ExprNode& Function::operator()(const ExprNode& arg1) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19),expr());
}
const ExprNode& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19, const ExprNode& arg20) const {
	//return ExprApply::new_(*this,Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20),expr());
	return ExprCopy().copy(args(),Array<const ExprNode>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20),expr());
}

const ExprNode& Function::operator()(const vector<const ExprNode*>& arg) const {
	return ExprCopy().copy(args(),arg, expr());
}

const ExprNode& Function::operator()(const Array<const ExprNode>& new_args) const {
	assert(nb_arg()==new_args.size());
//	return ExprApply::new_(*this,args);
	return ExprCopy().copy(args(),new_args,expr());
}

#define CONCAT(a,b)         CONCAT_HIDDEN(a,b)
#define CONCAT_HIDDEN(a,b)  a ## b
#define _I(ref,i) ExprConstant::new_scalar(CONCAT(arg,i))
#define _V(ref,i) ExprConstant::new_vector(CONCAT(arg,i), arg(i).type()==Dim::ROW_VECTOR)
#define _M(ref,i) ExprConstant::new_matrix(CONCAT(arg,i))

//const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1)       { return (*this)(arg0,_I(0,1)); }


IntervalMatrix Function::eval_matrix(const IntervalVector& box) const {
	// --> commented to avoid treating each component separately
	// (note that in this case, the root node of the expression is not evaluated)
	//return eval_matrix(box, BitSet::all(_image_dim.nb_rows()));
	// --------------------------------------------------

	IntervalMatrix M(_image_dim.nb_rows(),_image_dim.nb_cols());

	switch (expr().dim.type()) {
	case Dim::SCALAR     :
		M[0][0]=eval_domain(box).i();
		break;
	case Dim::ROW_VECTOR :
		M.set_row(0,eval_vector(box));
		break;
	case Dim::COL_VECTOR :
		M.set_col(0,eval_vector(box));
		break;
	case Dim::MATRIX:
		M=((Function*) this)->_eval->eval(box).m();
		break;
	default :
		throw std::logic_error("Function::eval_matrix: invalid Dim type");
	}

	return M;
}

IntervalMatrix Function::eval_matrix(const IntervalVector& box, const BitSet& rows) const {
	assert(!rows.empty());
	assert(rows.max()<_image_dim.nb_rows());

	IntervalMatrix M(rows.size(),_image_dim.nb_cols());

	switch (expr().dim.type()) {
	case Dim::SCALAR     :
		M[0][0]=eval_domain(box).i();
		break;
	case Dim::ROW_VECTOR :
		M.set_row(0,eval_vector(box));
		break;
	case Dim::COL_VECTOR :
		M.set_col(0,eval_vector(box,rows));
		break;
	case Dim::MATRIX:
		if (rows.size()==1)
			M.set_row(0,((Function*) this)->_eval->eval(box,rows).v());
		else
			M=((Function*) this)->_eval->eval(box,rows).m();
		break;
	default :
		throw std::logic_error("Function::eval_matrix: invalid Dim type");
	}

	return M;
}

IntervalMatrix Function::eval_matrix(const IntervalVector& box, const BitSet& rows, const BitSet& cols) const {
	assert(!rows.empty());
	assert(!cols.empty());
	assert(rows.max()<_image_dim.nb_rows());
	assert(cols.max()<_image_dim.nb_cols());

	IntervalMatrix M(rows.size(), cols.size());

	switch (expr().dim.type()) {
	case Dim::SCALAR     :
		M[0][0]=eval_domain(box).i();
		break;
	case Dim::ROW_VECTOR :
		M.set_row(0,eval_vector(box,cols));
		break;
	case Dim::COL_VECTOR :
		M.set_col(0,eval_vector(box,rows));
		break;
	case Dim::MATRIX:
		if (rows.size()==1)
			if (cols.size()==1)
				M[0][0]=((Function*) this)->_eval->eval(box,rows,cols).i();
			else
				M.set_row(0,((Function*) this)->_eval->eval(box,rows,cols).v());
		else
			if (cols.size()==1)
				M.set_col(0,((Function*) this)->_eval->eval(box,rows,cols).v());
			else
		        M=((Function*) this)->_eval->eval(box,rows,cols).m();
		break;
	default :
		throw std::logic_error("Function::eval_matrix: invalid Dim type");
	}

	return M;
}

} // namespace ibex
