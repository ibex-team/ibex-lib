//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprExprGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include "ibex_P_ExprGenerator.h"
#include "ibex_SyntaxError.h"
#include "ibex_Exception.h"
#include "ibex_Expr.h"
#include "ibex_ExprDiff.h"

#include <sstream>

using namespace std;

namespace ibex {

namespace parser {

extern stack<Scope>& scopes();

class LabelNode : public Label {
public:
	LabelNode(const ExprNode* node) : _node(node) { }

	virtual ~LabelNode() { }

	virtual const ExprNode& node() const { return *_node; }

	virtual bool is_const() const { return false; }

	virtual const Domain& domain() const {
		throw SyntaxError("Unexpected symbol inside constant expression");
	}

	virtual const Dim& dim() const { return _node->dim; }

	const ExprNode* _node;
};

class LabelConst : public Label {
public:
	// do not use POS_INFINITY to avoid confusion
	typedef enum { NEG_INF=0, POS_INF=1, OTHER=2 } number_type;

	// This constant cannot be represented by a Domain
	// object because (+oo,+oo) is automatically replaced by the empty set.
	static LabelConst* pos_infinity() {
		return new LabelConst(POS_INF);
	}

	static LabelConst* neg_infinity() {
		return new LabelConst(NEG_INF);
	}

	LabelConst(int v) : _domain(Dim::scalar()), num_type(OTHER), cst(NULL) {
		_domain.i()=Interval(v,v);
	}

	LabelConst(const Interval& itv) : _domain(Dim::scalar()), num_type(OTHER), cst(NULL) {
		_domain.i()=itv;
	}

	LabelConst(const Domain& d, bool ref=false) : _domain(d,ref), num_type(OTHER), cst(NULL) {

	}

	// Label of a constant symbol.
//	LabelConst(std::pair<const ExprConstant*, const Domain*>& p) : _domain(*p.second,true), num_type(OTHER), cst(p.first) {
//
//	}
//
	LabelConst(const ExprConstant& node) : _domain(node.get(),true), num_type(OTHER), cst(NULL) {

	}


	virtual const ExprNode& node() const {

		if (cst==NULL) {
			if (num_type!=OTHER)
				throw SyntaxError("Unexpected infinity symbol \"oo\"");

			// The final node domain is *not* a reference to
			// the constant domain: all objects created
			// during parsing can be safely deleted

			((LabelConst*) this)->cst = &ExprConstant::new_(_domain, false);

		}
		return *cst;
	}

	virtual bool is_const() const { return true; }

	virtual const Domain& domain() const { return _domain; }

	virtual const Dim& dim() const { return _domain.dim; }

	Domain _domain;
	number_type num_type;
	const ExprConstant* cst; // only built if necessary

private:
	LabelConst(number_type num_type) : _domain(Dim::scalar()), num_type(num_type), cst(NULL) {
		_domain.set_empty();
	}

};

int to_integer(const Domain& d) {
	assert(d.dim.is_scalar());
	assert(d.i().is_degenerated());
	double x=d.i().mid();
	assert(floor(x)==x);
	return (int)x;
}

double to_double(const Domain& d, bool round_downward) {
	assert(d.dim.is_scalar());
	// Note: requiring d.i().is_degenerated() is wrong,
	// the result of a calculus with degenerated intervals
	// may not be degenerated (and large).
	// Second, one may use Minibex interval constant like "pi"
	// in a bound of an interval, ex:
	// variables
	//     x in [0,pi].
	return round_downward? d.i().lb() : d.i().ub();
}

ExprGenerator::ExprGenerator() : scope(scopes().top()) {

}

Domain ExprGenerator::generate_cst(const P_ExprNode& y) {
	visit(y);
	Domain d=y.lab->domain();

	// cleanup important in case of the same expression
	// evaluated several times in different context
	// (iterator values changing)
	y.cleanup();
	return d;
}

int ExprGenerator::generate_int(const P_ExprNode& y) {
	return to_integer(generate_cst(y));
}

double ExprGenerator::generate_dbl(const P_ExprNode& y, bool round_downward) {
	visit(y);
	const Domain& d=y.lab->domain();
	double value;

	switch(((LabelConst*) y.lab)->num_type) {
	case LabelConst::NEG_INF:
		value=NEG_INFINITY;
		break;
	case LabelConst::POS_INF:
		value=POS_INFINITY;
		break;
	default:
		value=to_double(d, round_downward);
	}
	y.cleanup();
	return value;
}

const ExprNode& ExprGenerator::generate(const P_ExprNode& y) {
//	// create the nodes for the constants symbols
//	// by default, all constants are used (see destruction below)
//	int m=scope.cst.size();
//	Array<const ExprConstant> csts(m);
//	for (int i=0; i<m; i++) {
//		const char* id=scope.cst[i];
//		// Domains passed by copy
//		csts.set_ref(i,ExprConstant::new_(*scope.get_cst(id).second));
//		scope.bind_cst_node(id, csts[i]);
//	}

	visit(y);
	const ExprNode& e=y.lab->node();

	// cleanup important in case of the same expression
	// evaluated several times in different context
	// (iterator values changing)
	y.cleanup();

	// destroy unused constants
//	for (int i=0; i<m; i++) {
//		if (csts[i].fathers.is_empty() // no father
//				&& (&csts[i]!=&(y.lab->node())) // and not root node
//		)
//			delete &csts[i];
//	}

	return e;
}

void ExprGenerator::visit(const P_ExprNode& e) {

	if (e.lab!=NULL) return;

	if (e.op==P_ExprNode::POWER ||
		e.op==P_ExprNode::EXPR_WITH_IDX) {
		e.acceptVisitor(*this);
		return;
	}

	Array<const Domain> arg_cst(e.arg.size());

	bool all_cst=true;
	for (int i=0; i<e.arg.size(); i++) {
		visit(e.arg[i]);
		all_cst = all_cst && (e.arg[i].lab->is_const());
		if (all_cst) arg_cst.set_ref(i,e.arg[i].lab->domain());
	}

	if (all_cst) {

		if (e.op==P_ExprNode::MINUS) {
			LabelConst& c=*((LabelConst*) e.arg[0].lab);
			switch(c.num_type) {
			case LabelConst::POS_INF:
				e.lab = LabelConst::neg_infinity();
				break;
			case LabelConst::NEG_INF:
				e.lab = LabelConst::pos_infinity();
				break;
			default:
				e.lab = new LabelConst(-arg_cst[0]);
			}
			return;
		}

		try {
			switch(e.op) {
			case P_ExprNode::INFTY:      e.lab=LabelConst::pos_infinity(); break;
			case P_ExprNode::VAR_SYMBOL: e.lab=new LabelNode(scope.get_var(((P_ExprVarSymbol&) e).name).first); break;
			case P_ExprNode::CST_SYMBOL: e.lab=new LabelConst(scope.get_cst(((P_ExprCstSymbol&) e).name)); break;
			case P_ExprNode::TMP_SYMBOL:
				{
					const P_ExprNode& e2=scope.get_expr_tmp_expr(((P_ExprTmpSymbol&) e).name);
					visit(e2);
					e.lab = new LabelNode(&e2.lab->node());
				}
				break;
			case P_ExprNode::CST:     e.lab=new LabelConst(((P_ExprConstant&) e).value,true); break;
			case P_ExprNode::ITER:    e.lab=new LabelConst(scope.get_iter_value(((P_ExprIter&) e).name)); break;
			case P_ExprNode::IDX:
			case P_ExprNode::IDX_RANGE:
			case P_ExprNode::IDX_ALL:
			case P_ExprNode::EXPR_WITH_IDX:  assert(false); /* impossible */ break;
			case P_ExprNode::ROW_VEC:  e.lab=new LabelConst(Domain(arg_cst,true)); break;
			case P_ExprNode::COL_VEC:  e.lab=new LabelConst(Domain(arg_cst,false)); break;
			case P_ExprNode::APPLY:    e.lab=new LabelConst((((const P_ExprApply&) e).f).basic_evaluator().eval(arg_cst)); break;
			case P_ExprNode::CHI:      not_implemented("Chi in constant expressions"); break;
			case P_ExprNode::ADD:      e.lab=new LabelConst(arg_cst[0] + arg_cst[1]); break;
			case P_ExprNode::MUL:      e.lab=new LabelConst(arg_cst[0] * arg_cst[1]); break;
			case P_ExprNode::SUB:      e.lab=new LabelConst(arg_cst[0] - arg_cst[1]); break;
			case P_ExprNode::DIV:      e.lab=new LabelConst(arg_cst[0] / arg_cst[1]); break;
			case P_ExprNode::MAX:      e.lab=new LabelConst(max(arg_cst)); break;
			case P_ExprNode::MIN:      e.lab=new LabelConst(min(arg_cst)); break;
			case P_ExprNode::ATAN2:    e.lab=new LabelConst(atan2(arg_cst[0], arg_cst[1])); break;
			case P_ExprNode::POWER:
			case P_ExprNode::MINUS:    assert(false); /* impossible */ break;
			case P_ExprNode::UNARY_OP: e.lab=new LabelConst(ExprGenericUnaryOp::get_eval(((const P_ExprGenericUnaryOp&) e).name)(arg_cst[0])); break;
			case P_ExprNode::BINARY_OP:e.lab=new LabelConst(ExprGenericBinaryOp::get_eval(((const P_ExprGenericBinaryOp&) e).name)(arg_cst[0],arg_cst[1])); break;
			case P_ExprNode::TRANS:    e.lab=new LabelConst(transpose(arg_cst[0])); break;
			case P_ExprNode::SIGN:     e.lab=new LabelConst(sign (arg_cst[0])); break;
			case P_ExprNode::ABS:      e.lab=new LabelConst(abs  (arg_cst[0])); break;
			case P_ExprNode::SQR:      e.lab=new LabelConst(sqr  (arg_cst[0])); break;
			case P_ExprNode::SQRT:     e.lab=new LabelConst(sqrt (arg_cst[0])); break;
			case P_ExprNode::EXP:      e.lab=new LabelConst(exp  (arg_cst[0])); break;
			case P_ExprNode::LOG:      e.lab=new LabelConst(log  (arg_cst[0])); break;
			case P_ExprNode::COS:      e.lab=new LabelConst(cos  (arg_cst[0])); break;
			case P_ExprNode::SIN:      e.lab=new LabelConst(sin  (arg_cst[0])); break;
			case P_ExprNode::TAN:      e.lab=new LabelConst(tan  (arg_cst[0])); break;
			case P_ExprNode::ACOS:     e.lab=new LabelConst(acos (arg_cst[0])); break;
			case P_ExprNode::ASIN:     e.lab=new LabelConst(asin (arg_cst[0])); break;
			case P_ExprNode::ATAN:     e.lab=new LabelConst(atan (arg_cst[0])); break;
			case P_ExprNode::COSH:     e.lab=new LabelConst(cosh (arg_cst[0])); break;
			case P_ExprNode::SINH:     e.lab=new LabelConst(sinh (arg_cst[0])); break;
			case P_ExprNode::TANH:     e.lab=new LabelConst(tanh (arg_cst[0])); break;
			case P_ExprNode::ACOSH:    e.lab=new LabelConst(acosh(arg_cst[0])); break;
			case P_ExprNode::ASINH:    e.lab=new LabelConst(asinh(arg_cst[0])); break;
			case P_ExprNode::ATANH:    e.lab=new LabelConst(atanh(arg_cst[0])); break;
			case P_ExprNode::DIFF:
				throw SyntaxError("\"diff\" cannot be applied to constants"); break;
			case P_ExprNode::INF:
				if (!arg_cst[0].dim.is_scalar()) throw DimException("\"inf\" expects an interval as argument");
				e.lab=new LabelConst(arg_cst[0].i().lb()); break;
			case P_ExprNode::MID:
				if (!arg_cst[0].dim.is_scalar()) throw DimException("\"mid\" expects an interval as argument");
				e.lab=new LabelConst(arg_cst[0].i().mid()); break;
			case P_ExprNode::SUP:
				if (!arg_cst[0].dim.is_scalar()) throw DimException("\"sup\" expects an interval as argument");
				e.lab=new LabelConst(arg_cst[0].i().ub()); break;
			}
		} catch(DimException& exc) {
			throw SyntaxError(exc.message(),NULL,e.line);
		}
		return;
	}

	Array<const ExprNode> arg_node(e.arg.size());

	for (int i=0; i<e.arg.size(); i++) {
		// may force constants to become nodes
		arg_node.set_ref(i,e.arg[i].lab->node());
	}

	const ExprNode* node;

	try {
		switch(e.op) {
		case P_ExprNode::INFTY:
		case P_ExprNode::VAR_SYMBOL:
		case P_ExprNode::CST_SYMBOL:
		case P_ExprNode::TMP_SYMBOL:
		case P_ExprNode::CST:
		case P_ExprNode::ITER:
		case P_ExprNode::IDX:
		case P_ExprNode::IDX_RANGE:
		case P_ExprNode::IDX_ALL:
		case P_ExprNode::EXPR_WITH_IDX: assert(false); /* impossible */ break;
		case P_ExprNode::ROW_VEC:   node=&ExprVector::new_row(arg_node); break;
		case P_ExprNode::COL_VEC:   node=&ExprVector::new_col(arg_node); break;
		case P_ExprNode::APPLY:     node=&(((const P_ExprApply&) e).f)(arg_node); break;
		case P_ExprNode::CHI:       node=&chi(arg_node[0], arg_node[1], arg_node[2]); break;
		case P_ExprNode::ADD:       node=&(arg_node[0] + arg_node[1]); break;
		case P_ExprNode::MUL:       node=&(arg_node[0] * arg_node[1]); break;
		case P_ExprNode::SUB:       node=&(arg_node[0] - arg_node[1]); break;
		case P_ExprNode::DIV:       node=&(arg_node[0] / arg_node[1]); break;
		case P_ExprNode::MAX:       node=&max(arg_node); break;
		case P_ExprNode::MIN:       node=&min(arg_node); break;
		case P_ExprNode::ATAN2:     node=&atan2(arg_node[0], arg_node[1]); break;
		case P_ExprNode::POWER:     assert(false); /* impossible */ break;
		case P_ExprNode::MINUS:     node=&(-arg_node[0]); break;
		case P_ExprNode::UNARY_OP:  node=&ExprGenericUnaryOp::new_(((const P_ExprGenericUnaryOp&) e).name, arg_node[0]); break;
		case P_ExprNode::BINARY_OP: node=&ExprGenericBinaryOp::new_(((const P_ExprGenericBinaryOp&) e).name, arg_node[0], arg_node[1]); break;
		case P_ExprNode::TRANS:     node=&transpose(arg_node[0]); break;
		case P_ExprNode::SIGN:      node=&sign (arg_node[0]); break;
		case P_ExprNode::ABS:       node=&abs  (arg_node[0]); break;
		case P_ExprNode::SQR:       node=&sqr  (arg_node[0]); break;
		case P_ExprNode::SQRT:      node=&sqrt (arg_node[0]); break;
		case P_ExprNode::EXP:       node=&exp  (arg_node[0]); break;
		case P_ExprNode::LOG:       node=&log  (arg_node[0]); break;
		case P_ExprNode::COS:       node=&cos  (arg_node[0]); break;
		case P_ExprNode::SIN:       node=&sin  (arg_node[0]); break;
		case P_ExprNode::TAN:       node=&tan  (arg_node[0]); break;
		case P_ExprNode::ACOS:      node=&acos (arg_node[0]); break;
		case P_ExprNode::ASIN:      node=&asin (arg_node[0]); break;
		case P_ExprNode::ATAN:      node=&atan (arg_node[0]); break;
		case P_ExprNode::COSH:      node=&cosh (arg_node[0]); break;
		case P_ExprNode::SINH:      node=&sinh (arg_node[0]); break;
		case P_ExprNode::TANH:      node=&tanh (arg_node[0]); break;
		case P_ExprNode::ACOSH:     node=&acosh(arg_node[0]); break;
		case P_ExprNode::ASINH:     node=&asinh(arg_node[0]); break;
		case P_ExprNode::ATANH:     node=&atanh(arg_node[0]); break;
		case P_ExprNode::DIFF:      node=&diff (arg_node); break;
		case P_ExprNode::INF:       throw SyntaxError("\"inf\" operator requires constant interval"); break;
		case P_ExprNode::MID:       throw SyntaxError("\"mid\" operator requires constant interval"); break;
		case P_ExprNode::SUP:       throw SyntaxError("\"sup\" operator requires constant interval"); break;
		}
		e.lab = new LabelNode(node);
	} catch(DimException& exc) {
		throw SyntaxError(exc.message(),NULL,e.line);
	}
}

void ExprGenerator::visit(const P_ExprPower& e) {

	visit(e.arg[0]);
	visit(e.arg[1]);

	Label& left=(*(e.arg[0].lab));
	Label& right=(*(e.arg[1].lab));

	typedef enum { IBEX_INTEGER, IBEX_INTERVAL, IBEX_EXPRNODE } _type;
	_type right_type;
	_type left_type;

	int int_right=0;
	Interval itv_right;
	Interval itv_left;

	if (right.is_const()) {
		if (!right.domain().dim.is_scalar()) throw SyntaxError("exponent must be scalar");

		right_type=IBEX_INTERVAL;
		itv_right=right.domain().i();
		//delete cr; // not now (see comment in ExprCopy.h)
		// NOTE: we can delete cr because
		// even in the case where right_type==IBEX_INTERVAL and left_type==IBEX_EXPRNODE
		// we will recreate a ExprConstant node by multiplying itv_right (instead of RIGHT)
		// with LEFT.

		// try to see if the exponent is an integer
		if (itv_right.is_degenerated()) {
			double x=itv_right.mid();
			if (floor(x)==x) {
				right_type=IBEX_INTEGER;
				int_right=(int)floor(x);
			}
		}
	} else
		right_type=IBEX_EXPRNODE;


	if (left.is_const()) {
		left_type=IBEX_INTERVAL;
		itv_left=left.domain().i();
		//delete cl; // LEFT will no longer be used // not now (see comment in ExprCopy.h)
	} else
		left_type=IBEX_EXPRNODE;


	if (left_type==IBEX_INTERVAL) {
		if (right_type==IBEX_INTEGER) {
			e.lab=new LabelConst(pow(itv_left,int_right));
		} else if (right_type==IBEX_INTERVAL) {
			e.lab=new LabelConst(pow(itv_left,itv_right));
		} else {
			e.lab=new LabelNode(&exp(right.node() * log(itv_left))); // *log(...) will create a new ExprConstant.
		}
	}  else {
		if (right_type==IBEX_INTEGER) {
			e.lab=new LabelNode(&pow(left.node(),int_right));
		} else if (right_type==IBEX_INTERVAL) {
			e.lab=new LabelNode(&exp(itv_right * log(left.node())));
		} else {
			e.lab=new LabelNode(&exp(right.node() * log(left.node())));
		}
	}
}

pair<int,int> ExprGenerator::visit_index_tmp(const Dim& dim, const P_ExprNode& idx, bool matlab_style) {
	int i1,i2;

	switch(idx.op) {
	case P_ExprNode::IDX_ALL :
		assert(idx.arg.size()==0);
		i1=i2=-1;
		break;
	case P_ExprNode::IDX_RANGE :
		assert(idx.arg.size()==2);
		visit(idx.arg[0]);
		visit(idx.arg[1]);
		assert(idx.arg[0].lab->is_const());
		assert(idx.arg[1].lab->is_const());
		i1=to_integer(idx.arg[0].lab->domain());
		i2=to_integer(idx.arg[1].lab->domain());
		if (matlab_style) { i1--; i2--; }
		if (i1<0 || i2<0)
			throw SyntaxError("negative index. Note: indices in Matlab-style (using parenthesis like in \"x(i)\") start from 1 (not 0).");
		break;
	case P_ExprNode::IDX :
		assert(idx.arg.size()==1);
		visit(idx.arg[0]);
		assert(idx.arg[0].lab->is_const());
		i1=i2=to_integer(idx.arg[0].lab->domain());
		if (matlab_style) { i1--; i2--; }
		if (i1<0)
			throw SyntaxError("negative index. Note: indices in Matlab-style (using parenthesis like in \"x(i)\") start from 1 (not 0).");
		break;
	default:
		assert(false);
	}

	/**
	 * To be clean, we should create a LabeNode subclass
	 * that contains the returned pair. Meanwhile, we
	 * have to create a dummy label so that
	 * the cleanup will not consider this node as already visited
	 * (and delete properly the labels of the index/indices).
	 */
	idx.lab = new LabelConst(0); // dummy

	return pair<int,int>(i1,i2);
}

DoubleIndex ExprGenerator::visit_index(const Dim& dim, const P_ExprNode& idx1, bool matlab_style) {

	pair<int,int> p=visit_index_tmp(dim,idx1,matlab_style);

	// If only one index is supplied
	// it may corresponds to a row number or
	// a column number (in case of row vector).
	if (p.first==-1) {
		return DoubleIndex::all(dim);
	} else if (p.second==p.first) {
		return DoubleIndex::one_index(dim,p.first);
	} else {
		if (dim.nb_rows()>1)
			return DoubleIndex::rows(dim,p.first,p.second);
		else
			return DoubleIndex::cols(dim,p.first,p.second);
	}
}

DoubleIndex ExprGenerator::visit_index(const Dim& dim, const P_ExprNode& idx1, const P_ExprNode& idx2, bool matlab_style) {

	pair<int,int> r=visit_index_tmp(dim,idx1,matlab_style);
	pair<int,int> c=visit_index_tmp(dim,idx2,matlab_style);

	if (r.first==-1) {
		if (c.first==-1)
			return DoubleIndex::all(dim);
		else if (c.first==c.second)
			return DoubleIndex::one_col(dim,c.first);
		else
			return DoubleIndex::cols(dim,c.first,c.second);
	} else if (r.first==r.second) {
		if (c.first==-1)
			return DoubleIndex::one_row(dim,r.first);
		else if (c.first==c.second)
			return DoubleIndex::one_elt(dim,r.first,c.first);
		else
			return DoubleIndex::subrow(dim,r.first,c.first,c.second);
	} else {
		if (c.first==-1)
			return DoubleIndex::rows(dim,r.first,r.second);
		else if (c.first==c.second)
			return DoubleIndex::subcol(dim,r.first,r.second,c.first);
		else
			return DoubleIndex::submatrix(dim,r.first,r.second,c.first,c.second);
	}
}

void ExprGenerator::visit(const P_ExprWithIndex& e) {
	visit(e.arg[0]);

	Label& expr=(*(e.arg[0].lab));

	DoubleIndex idx=e.arg.size()==2?
			visit_index(expr.dim(), e.arg[1], e.matlab_style)
			:
			visit_index(expr.dim(), e.arg[1], e.arg[2], e.matlab_style);

	if (expr.is_const()) {
		Domain d=expr.domain()[idx];
		e.lab = new LabelConst(d,d.is_reference);
	} else {
		e.lab = new LabelNode(&ExprIndex::new_(expr.node(),idx));
	}
}

const ExprNode& ExprGenerator::diff(const Array<const ExprNode>& args) {
	assert(args.size()>1);

	// get "y"
	const ExprNode& y=args[0];

	// get the expressions "x"
	// (removing the first node, which is "y")
	Array<const ExprSymbol> x(args.size()-1);
	for (int i=0; i<args.size()-1; i++) {
		const ExprSymbol* xi=dynamic_cast<const ExprSymbol*>(&args[i+1]);
		if (!xi) {
			throw SyntaxError("\"diff\" can only be applied to symbols");
		}
		x.set_ref(i,*xi);
	}
	return ExprDiff().diff(y,x);
}

} // end namespace parser
} // end namespace ibex
