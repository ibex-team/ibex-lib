//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExpr.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 25, 2012
// Last Update : May 05, 2016
//============================================================================

#ifndef __IBEX_PARSER_EXPR_H__
#define __IBEX_PARSER_EXPR_H__

#include <vector>
#include "ibex_Domain.h"
#include "ibex_P_ExprVisitor.h"
#include "ibex_Entity.h"
#include <cassert>

namespace ibex {

class Expr;

namespace parser {


class Label {
public:
	typedef enum { NODE, CONST, INDEX } label_type;
	virtual const ExprNode& node()=0;

	const Domain& domain() {
		assert(type()==CONST);
		return *((LabelConst&) *this).domain;
	}

	virtual ~Label() { }
	virtual label_type type()=0;
};

class P_ExprNode {
public:
	typedef enum {
		// infinity constant
		// This constant cannot be represented by an #ibex::P_ExprConstant
		// object because (+oo,+oo) is automatically replaced by the empty set.
		INF,
		SYMBOL,
		CST,  // constant
		ITER, // iterator symbol
		IDX, IDX_RANGE, IDX_ALL,
		EXPR_WITH_IDX,
		ROW_VEC, COL_VEC, APPLY, CHI,
		ADD, MUL, SUB, DIV, MAX, MIN, ATAN2, POWER,
		MINUS, TRANS, SIGN, ABS,
		SQR, SQRT, EXP, LOG,
		COS,  SIN,  TAN,  ACOS,  ASIN,  ATAN,
		COSH, SINH, TANH, ACOSH, ASINH, ATANH,
	} operation;

	P_ExprNode(operation op) : op(op), lab(NULL) { }

	P_ExprNode(operation op, const P_ExprNode& arg1) : op(op), lab(NULL) { }

	P_ExprNode(operation op, const P_ExprNode& arg1, const P_ExprNode& arg2) : op(op), arg(arg1,arg2), lab(NULL) { }

	P_ExprNode(operation op, const P_ExprNode& arg1, const P_ExprNode& arg2, const P_ExprNode& arg3) : op(op), lab(NULL) { }

	P_ExprNode(operation op, const Array<const P_ExprNode>& arg) : op(op), arg(arg), lab(NULL) { }

	virtual ~P_ExprNode() { }

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	operation op;
	Array<const P_ExprNode> arg;
	Label* lab;
};

class LabelNode : public Label {
public:
	virtual const ExprNode& node() { return *_node; }

	virtual label_type type() { return NODE; }
	const ExprNode* _node;
};

//class LabelConstNode : public Label {
//public:
//	virtual label_type type() { return CONST_NODE; }
//	const ExprConstant* cst;
//};

class LabelConst : public Label {
public:
	// do not use POS_INFINITY to avoid confusion
	typedef enum { NEG_INF=0, POS_INF=1, OTHER=2 } number_type;

	static LabelConst pos_infinity() {
		return LabelConst(POS_INF);
	}

	static LabelConst neg_infinity() {
		return LabelConst(NEG_INF);
	}

	LabelConst(int v) : domain(Dim::scalar()), num_type(OTHER), cst(NULL) {
		domain.i()=Interval(v,v);
	}

	LabelConst(const Interval& itv) : domain(Dim::scalar()), num_type(OTHER), cst(NULL) {
		domain.i()=itv;
	}

	LabelConst(const Domain& d) : domain(d), num_type(OTHER), cst(NULL) { }

	virtual const ExprNode& node() {
		if (!node) {
			node = new ExprConstant::new_(domain, false);
		}
		return *node;
	}

	virtual label_type type() { return CONST; }

	Domain domain;
	number_type num_type;
	const ExprConstant* cst; // only set if necessary

private:
	LabelConst(number_type num_type) : domain(Dim::scalar()), num_type(num_type), cst(NULL) {
		domain.set_empty();
	}

};
//
//class LabelInteger : public Label {
//public:
//	LabelInteger(int itg) : itg(itg) { }
//	virtual label_type type() { return INT; }
//	int itg;
//};
//
//class LabelDouble : public Label {
//public:
//	LabelDouble(int dbl) : dbl(dbl) { }
//	virtual label_type type() { return DOUBLE; }
//	double dbl;
//};

//class LabelDoubleIndex: public Label {
//public:
//	virtual label_type type() { return DOUBLE_INDEX; }
//	DoubleIndex idx;
//};

class LabelIndex : public Label {
public:
	virtual label_type type() { return INDEX; }
	std::pair<int,int> range;
};

/**
 * \brief Indexed expression (for parser only).
 *
 * An indexed expression "at parse time" can be
 * something like x[i+1].
 *
 * If matlab_style is true, the index starts from 1;
 * otherwise it starts from 0.
 */
class P_ExprIndex : public P_ExprNode {
public:
	P_ExprIndex(const P_ExprNode& expr, const P_ExprNode& single_idx, bool matlab_style);

	P_ExprIndex(const P_ExprNode& expr, const P_ExprNode& row, const P_ExprNode& col, bool matlab_style);

	virtual ~P_ExprIndex();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	bool matlab_style;
};

/*
 * SYB: At parse time, the domains of constants
 * built from symbols are references to same
 * object to avoid duplication (consider, e.g., an interval
 * matrix M appearing in many expressions).
 *
 * All the occurrences of the constant symbol
 * are associated to the same P_ExprConstant
 * object. See Scopes::get_cst().
 */
class P_ExprSymbol : public P_ExprNode {
public:

	P_ExprSymbol(const char* name, const Dim& dim, const Domain& domain);

	P_ExprSymbol(const char* name, const Dim& dim, const Interval& x);

	~P_ExprSymbol();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	// relics from Ibex 1.0
	typedef enum { VAR, EPR, SYB } Type;

	const char* name;
	Domain domain;
	Type type; // not specified at construction (see parser.yc)
};

/**
 * \brief Constant expressions (for parser only)
 *
 */
class P_ExprConstant : public P_ExprNode {
public:
	// should we add the name (for display?)
	// but then we have to build names like "x[i]"
	// when we build an ExprConstantRef from "x"
	// in ExprGenerator::visit(const P_ExprIndex&).
	P_ExprConstant(const Domain& d);

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	Domain value;
};

/**
 * \brief Iterator symbol (for parser only).
 */
class P_ExprIter : public P_ExprNode {
public:
	P_ExprIter(const char* name);

	virtual ~P_ExprIter();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};

class P_ExprApply : public P_ExprNode {
public:
	P_ExprApply(const Function& f, const Array<const P_ExprNode>* args);

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	Function& f;
};

void p_print(const P_ExprNode& e);

const P_ExprNode* apply(Function& f, const P_ExprNode* expr);

const P_ExprNode* apply(Function& f, const std::vector<const P_ExprNode*>* args);


/*================================== inline implementations ========================================*/

inline const P_ExprNode* index(const P_ExprNode* expr, const P_ExprNode* idx, bool matlab) {
	return new P_ExprIndex(*expr, *idx, matlab);
}

inline const P_ExprNode* index(const P_ExprNode* expr, const P_ExprNode* idx1, const P_ExprNode* idx2, bool matlab) {
	return new P_ExprIndex(*expr, *idx1, *idx2, matlab);
}

inline const P_ExprNode* row_vec(const Array<const P_ExprNode>* args) {
	return new P_ExprNode(P_ExprNode::ROW_VEC,*args);
}

inline const P_ExprNode* col_vec(const Array<const P_ExprNode>* args) {
	return new P_ExprNode(P_ExprNode::COL_VEC,*args);
}

inline const P_ExprNode* infinity() {
	return new P_ExprNode(P_ExprNode::INF);
}

inline const P_ExprNode* operator+(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::ADD,*left,*right);
}

inline const P_ExprNode* operator-(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::SUB,*left,*right);
}

inline const P_ExprNode* operator*(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::MUL,*left,*right);
}

inline const P_ExprNode* operator/(const P_ExprNode* *left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::DIV,*left,*right);
}

inline const P_ExprNode* max(const Array<const P_ExprNode>* args) {
	return new P_ExprNode(P_ExprNode::MAX,*args);
}

inline const P_ExprNode* min(const Array<const P_ExprNode>* args) {
	return new P_ExprNode(P_ExprNode::MIN,*args);
}

inline const P_ExprNode* atan2(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::ATAN2,*left,*right);
}

inline const P_ExprNode* pow(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::POWER,*left,*right);
}

inline const P_ExprNode* chi(const P_ExprNode* exp1, const P_ExprNode* exp2, const P_ExprNode* exp3) {
	return new P_ExprNode(P_ExprNode::CHI,*exp1,*exp2,*exp3);
}

inline const P_ExprNode* sqr(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::SQR,*exp);
}

inline const P_ExprNode* sqrt(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::SQRT,*exp);
}

inline const P_ExprNode* exp(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::EXP,*exp);
}

inline const P_ExprNode* log(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::LOG,*exp);
}

inline const P_ExprNode* cos(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::COS,*exp);
}

inline const P_ExprNode* sin(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::SIN,*exp);
}

inline const P_ExprNode* tan(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::TAN,*exp);
}

inline const P_ExprNode* acos(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ACOS,*exp);
}

inline const P_ExprNode* asin(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ASIN,*exp);
}

inline const P_ExprNode* atan(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ATAN,*exp);
}

inline const P_ExprNode* cosh(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::COSH,*exp);
}

inline const P_ExprNode* sinh(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::SINH,*exp);
}

inline const P_ExprNode* tanh(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::TANH,*exp);
}

inline const P_ExprNode* acosh(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ACOSH,*exp);
}

inline const P_ExprNode* asinh(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ASINH,*exp);
}

inline const P_ExprNode* atanh(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ATANH,*exp);
}

inline const P_ExprNode* operator-(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::MINUS,*exp);
}

inline const P_ExprNode* transpose(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::TRANS,*exp);
}

inline const P_ExprNode* sign(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::SIGN,*exp);
}

inline const P_ExprNode* abs(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::ABS,*exp);
}


} // end namespace parser

} // end namespace ibex

#endif // __IBEX_PARSER_EXPR_H__
