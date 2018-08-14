//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserNumConstraint.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_NUM_CONSTRAINT_H__
#define __IBEX_PARSER_NUM_CONSTRAINT_H__

#include "ibex_CtrGenerator.h"
#include "ibex_NumConstraint.h"

namespace ibex {

namespace parser {

class P_NumConstraint {
public:
	virtual void acceptVisitor(CtrGenerator& g) const=0;

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	virtual ~P_NumConstraint() { }
};

class P_OneConstraint : public P_NumConstraint {
public:
	P_OneConstraint(const P_ExprNode* left, CmpOp op, const P_ExprNode* right);

	void acceptVisitor(CtrGenerator& g) const {
		g.visit(*this);
	}

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	~P_OneConstraint();

	const P_ExprNode& expr;
	CmpOp op;
};

class P_ConstraintList : public P_NumConstraint {
public:
	P_ConstraintList(std::vector<P_NumConstraint*>* ctrs);

	void acceptVisitor(CtrGenerator& g) const {
		g.visit(*this);
	}

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	~P_ConstraintList();

	std::vector<P_NumConstraint*>& ctrs;

};

class P_ConstraintLoop : public P_NumConstraint {
public:
	P_ConstraintLoop(const char* iter, const P_ExprNode* first_value, const P_ExprNode* last_value, std::vector<P_NumConstraint*>* ctrs);

	void acceptVisitor(CtrGenerator& g) const {
		g.visit(*this);
	}

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	~P_ConstraintLoop();

	const char* iter;
	const P_ExprNode& first_value;
	const P_ExprNode& last_value;

	P_ConstraintList ctrs;
};


/**
 * A thick equality g(x) in [..,...]
 * (to be transformed into two inequalities)
 */
class P_ThickEquality : public P_NumConstraint {
public:
	P_ThickEquality(const P_ExprNode* expr, const Interval& d);

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	~P_ThickEquality();

	void acceptVisitor(CtrGenerator& g) const {
		g.visit(*this);
	}

	const P_ExprNode& expr;
	Interval d;
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_NUM_CONSTRAINT_H__
