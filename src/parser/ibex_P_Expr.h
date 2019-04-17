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

#include "ibex_Domain.h"
#include "ibex_Function.h"

#include "ibex_P_ExprVisitor.h"

#include <vector>
#include <cassert>

extern int ibex_lineno;

namespace ibex {

class ExprNode;

namespace parser {

/**
 * \brief Data associated to each node.
 */
class Label {
public:
	virtual const ExprNode& node() const =0;

	virtual bool is_const() const =0;

	virtual const Dim& dim() const =0;

	virtual const Domain& domain() const =0;

	virtual ~Label() { }
};

/**
 * \brief Root class of expression nodes at parse time.
 *
 * Important notice: one specificity of a parser expression
 * (as compared to the ExprNode hierarchy) is that only
 * symbols nodes (P_ExprSymbol) can be shared. All the other
 * nodes have only one father. Otherwise, the destructor should
 * be implemented differently.
 *
 */
class P_ExprNode {
public:
	typedef enum {
		INFTY,		   // infinity constant
		VAR_SYMBOL,    // variable symbol
		CST_SYMBOL,    // constant symbol
		TMP_SYMBOL,    // function temporary symbol
		CST,           // numeric constant
		ITER,          // iterator symbol
		IDX,           // one index (i)
		IDX_RANGE,     // a range of indices (i:j)
		IDX_ALL,       // all indices (:)
		EXPR_WITH_IDX, // something(i1:i2,j1:j2)
		ROW_VEC, COL_VEC, APPLY, CHI,
		ADD, MUL, SUB, DIV, MAX, MIN, ATAN2, POWER, SUM,
		MINUS, TRANS, SIGN, ABS,
		SQR, SQRT, EXP, LOG,
		COS,  SIN,  TAN,  ACOS,  ASIN,  ATAN,
		COSH, SINH, TANH, ACOSH, ASINH, ATANH,
		INF, MID, SUP,  // deprecated??
		DIFF, UNARY_OP, BINARY_OP
	} operation;

	P_ExprNode(operation op) : op(op), lab(NULL), line(ibex_lineno) { }

	P_ExprNode(operation op, const P_ExprNode& arg1) : op(op), arg(arg1), lab(NULL), line(ibex_lineno) { }

	P_ExprNode(operation op, const P_ExprNode& arg1, const P_ExprNode& arg2) : op(op), arg(arg1,arg2), lab(NULL), line(ibex_lineno) { }

	P_ExprNode(operation op, const P_ExprNode& arg1, const P_ExprNode& arg2, const P_ExprNode& arg3) : op(op), arg(arg1,arg2,arg3), lab(NULL), line(ibex_lineno) { }

	P_ExprNode(operation op, const Array<const P_ExprNode>& arg) : op(op), arg(arg), lab(NULL), line(ibex_lineno) { }

//	P_ExprNode(operation op, const std::vector<const P_ExprNode*>& vec) : op(op), arg(vec.size()), lab(NULL), line(ibex_lineno) {
//		int i=0;
//		for (std::vector<const P_ExprNode*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
//			arg.set_ref(i++,**it);
//		}
//	}

	virtual ~P_ExprNode();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	// Generate the expression with the current scope.
	const ExprNode& generate() const;

	// destroy labels recursively
	void cleanup() const;

	int _2int() const;

	/*
	 * Convert the expression to a double and, if necessary,
	 * either round it downward (round_downward=true)
	 * or upward (round_downard=false).
	 */
	double _2dbl(bool round_downward) const;

	Domain _2domain() const;

	Interval _2itv() const;

	operation op;
	Array<const P_ExprNode> arg;
	mutable Label* lab;
	int line;
};

/**
 * \brief Indexed expression.
 *
 * An indexed expression "at parse time" can be
 * something like x(i+1) or x(:,j:j+2)
 *
 * If matlab_style is true, the index starts from 1;
 * otherwise it starts from 0 (deprecated syntax with
 * C-like brackets).
 */
class P_ExprWithIndex : public P_ExprNode {
public:
	P_ExprWithIndex(const P_ExprNode& expr, const P_ExprNode& single_idx, bool matlab_style);

	P_ExprWithIndex(const P_ExprNode& expr, const P_ExprNode& row, const P_ExprNode& col, bool matlab_style);

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	bool matlab_style;
};

/**
 * \brief Power expression.
 *
 * Defined as a separated subclass because needs a specific
 * (longer) code for generation due to the case of non-integer
 * exponents.
 */
class P_ExprPower : public P_ExprNode {
public:
	P_ExprPower(const P_ExprNode& expr, const P_ExprNode& power);

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }
};

/**
 * \brief Sum expression.
 *
 */
class P_ExprSum : public P_ExprNode {
public:
	P_ExprSum(const P_ExprNode& expr, const char* iter, const P_ExprNode& first_value, const P_ExprNode& last_value);

	~P_ExprSum();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* iter;
};

/*
 * \brief Variable Symbol at parse time
 *
 * \see Scopes::get_cst().
 */
class P_ExprVarSymbol : public P_ExprNode {
public:

	P_ExprVarSymbol(const char* name);

	~P_ExprVarSymbol();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};

/**
 * \brief Constant Symbol at parse time
 *
 * All the occurrences of the same constant symbol, during
 * the generation of an expression, will be associated to the
 * same ExprConstant object. This avoids heavy duplication of
 * domains (consider, e.g., a big interval matrix appearing
 * in many expressions).
 */
class P_ExprCstSymbol : public P_ExprNode {
public:

	P_ExprCstSymbol(const char* name);

	~P_ExprCstSymbol();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};

/**
 * \brief Function temporary symbol
 */
class P_ExprTmpSymbol : public P_ExprNode {
public:

	P_ExprTmpSymbol(const char* name);

	~P_ExprTmpSymbol();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};


/**
 * \brief Constant expressions.
 *
 * Corresponds to numeric (not symbolic) constants.
 */
class P_ExprConstant : public P_ExprNode {
public:
	/**
	 * \brief Build a constant (domain passed by copy).
	 */
	P_ExprConstant(int x);

	/**
	 * \brief Build a constant (domain passed by copy).
	 */
	P_ExprConstant(double x);

	/**
	 * \brief Build a constant (domain passed by copy).
	 */
	P_ExprConstant(const Interval& x);

	/**
	 * \brief Build a constant (domain passed by copy).
	 */
	P_ExprConstant(const Domain& d);

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	Domain value;
};

/**
 * \brief Iterator symbol.
 */
class P_ExprIter : public P_ExprNode {
public:
	P_ExprIter(const char* name);

	virtual ~P_ExprIter();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};

/**
 * \brief Function application.
 */
class P_ExprApply : public P_ExprNode {
public:
	P_ExprApply(const Function& f, const Array<const P_ExprNode>& args);

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const Function& f;
};

/**
 * \brief Unary generic operator.
 */
class P_ExprGenericUnaryOp : public P_ExprNode {
public:
	P_ExprGenericUnaryOp(const char* name, const P_ExprNode& expr);

	~P_ExprGenericUnaryOp();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};

/**
 * \brief Binary generic operator.
 */
class P_ExprGenericBinaryOp : public P_ExprNode {
public:
	P_ExprGenericBinaryOp(const char* name, const P_ExprNode& left, const P_ExprNode& right);

	~P_ExprGenericBinaryOp();

	virtual void acceptVisitor(P_ExprVisitor& v) const { v.visit(*this); }

	const char* name;
};

std::ostream& operator<<(std::ostream& os, const P_ExprNode&);

const P_ExprNode* apply(Function& f, const Array<const P_ExprNode>& args);

/*================================== inline implementations ========================================*/

inline const P_ExprNode* index(const P_ExprNode* i) {
	return new P_ExprNode(P_ExprNode::IDX, *i);
}

inline const P_ExprNode* index_range(const P_ExprNode* i1, const P_ExprNode* i2) {
	return new P_ExprNode(P_ExprNode::IDX_RANGE, *i1, *i2);
}

inline const P_ExprNode* index_all() {
	return new P_ExprNode(P_ExprNode::IDX_ALL);
}

inline const P_ExprNode* expr_with_index(const P_ExprNode* expr, const P_ExprNode* idx, bool matlab) {
	return new P_ExprWithIndex(*expr, *idx, matlab);
}

inline const P_ExprNode* expr_with_index(const P_ExprNode* expr, const P_ExprNode* idx1, const P_ExprNode* idx2, bool matlab) {
	return new P_ExprWithIndex(*expr, *idx1, *idx2, matlab);
}

inline const P_ExprNode* row_vec(const std::vector<const P_ExprNode*>* args) {
	return new P_ExprNode(P_ExprNode::ROW_VEC,*args);
}

inline const P_ExprNode* col_vec(const std::vector<const P_ExprNode*>* args) {
	return new P_ExprNode(P_ExprNode::COL_VEC,*args);
}

inline const P_ExprNode* dbl_infinity() {
	return new P_ExprNode(P_ExprNode::INFTY);
}

inline const P_ExprNode* operator+(const P_ExprNode& left, const P_ExprNode& right) {
	return new P_ExprNode(P_ExprNode::ADD,left,right);
}

inline const P_ExprNode* operator-(const P_ExprNode& left, const P_ExprNode& right) {
	return new P_ExprNode(P_ExprNode::SUB,left,right);
}

inline const P_ExprNode* operator*(const P_ExprNode& left, const P_ExprNode& right) {
	return new P_ExprNode(P_ExprNode::MUL,left,right);
}

inline const P_ExprNode* operator/(const P_ExprNode& left, const P_ExprNode& right) {
	return new P_ExprNode(P_ExprNode::DIV,left,right);
}

inline const P_ExprNode* max(const std::vector<const P_ExprNode*>* args) {
	return new P_ExprNode(P_ExprNode::MAX,*args);
}

inline const P_ExprNode* min(const std::vector<const P_ExprNode*>* args) {
	return new P_ExprNode(P_ExprNode::MIN,*args);
}

inline const P_ExprNode* atan2(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprNode(P_ExprNode::ATAN2,*left,*right);
}

inline const P_ExprNode* pow(const P_ExprNode* left, const P_ExprNode* right) {
	return new P_ExprPower(*left,*right);
}

inline const P_ExprNode* sum(const P_ExprNode* expr, const char* iter, const P_ExprNode* first_value, const P_ExprNode* last_value) {
	return new P_ExprSum(*expr , iter, *first_value, *last_value);
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

inline const P_ExprNode* operator-(const P_ExprNode& exp) {
	return new P_ExprNode(P_ExprNode::MINUS,exp);
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

inline const P_ExprNode* inf(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::INF,*exp);
}

inline const P_ExprNode* mid(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::MID,*exp);
}

inline const P_ExprNode* sup(const P_ExprNode* exp) {
	return new P_ExprNode(P_ExprNode::SUP,*exp);
}

inline const P_ExprNode* diff(const std::vector<const P_ExprNode*>* args) {
	return new P_ExprNode(P_ExprNode::DIFF,*args);
}


} // end namespace parser

} // end namespace ibex

#endif // __IBEX_PARSER_EXPR_H__
