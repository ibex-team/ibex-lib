//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprMonomial.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 30, 2020
// Last update : May 12, 2020
//============================================================================

#ifndef __IBEX_EXPR_MONOMIAL_H__
#define __IBEX_EXPR_MONOMIAL_H__

#include "ibex_Expr.h"
#include "ibex_ExprCmp.h"
#include "ibex_ExprOccCounter.h"

#include <list>

namespace ibex {

class ExprSimplify2;

/**
 * \ingroup symbolic
 *
 * Expression monomial (for simplification process)
 */
class ExprMonomial {
public:
	/**
	 * Create a single-term monomial e^power
	 *
	 * \param e     - sub-expression .
	 * \param power - power: taken into account
	 *                only if e is scalar.
	 */
	explicit ExprMonomial(const ExprNode& e, int power=1);

	/**
	 * Create a single-term constant monomial
	 */
	explicit ExprMonomial(const Interval& x);

	/**
	 * Create a single-term constant monomial
	 */
	explicit ExprMonomial(const IntervalVector& x, bool in_row);

	/**
	 * Create a single-term constant monomial
	 */
	explicit ExprMonomial(const IntervalMatrix& x);

	/**
	 * Copy m.
	 */
	ExprMonomial(const ExprMonomial& m);

	/**
	 * Delete this.
	 */
	~ExprMonomial();

	/**
	 * True if this monomial is a constant,
	 * including a constant matrix.
	 */
	bool is_constant() const;

	/**
	 * (*this)^2
	 */
	ExprMonomial square() const;

	/**
	 * To string
	 */
	operator std::string() const;

	/**
	 * Generates an equivalent expression
	 *
	 * New nodes are pushed in the record
	 * vector to ease memory cleanup, if
	 * provided. Otherwise: memory leaks!
	 */
	const ExprNode& to_expr(std::vector<const ExprNode*>* record=NULL) const;

	/**
	 * Multiplication.
	 *
	 * The ExprSimplify2 argument is necessary because
	 * the transposition of some expressions can be made
	 * to allow factorization of dot products.
	 * If NULL -> no simplification and memory leak in such
	 * cases.
	 */
	ExprMonomial mul(const ExprMonomial& m, ExprSimplify2* s=NULL) const;

	/**
	 * Count the total number of symbol occurrences.
	 */
	Matrix count_occ(const ExprOccCounter& c) const;

	/**
	 * Dimension of the monomial
	 */
	Dim dim() const;

	/**
	 * Leading scalar coefficient
	 */
	Interval coeff;

	/**
	 * Thrown when the result is not a monomial (ex: x+y)
	 */
	class NotAMonomial { };

	/**
	 * Thrown if the null monomial appears (not allowed)
	 */
	class NullResult { };

protected:
	class Term;

	/**
	 * Create an uninitialized monomial.
	 *
	 * Must be followed by a call to add_term
	 */
	ExprMonomial();

	/*
	 * Add a term at the end.
	 * Must respect term ordering (>last).
	 */
	void add_term(Term* t);

	/*
	 * Clear all terms
	 */
	void clear_terms();

	/*
	 * Insert a term at the right place.
	 */
	void insert(Term* t);

	/**
	 * Compare with a monomial m2 and
	 * add/sub both if m12!=NULL (m12 is the result).
	 *
	 * \param
	 *        - m12 : the monomial to be built (NULL if not required).
	 *          This monomial must be empty (i.e.,=zero) before calling this function.
	 *
	 * \return
	 *         - a int: -1, 0 or 1 if this<m2, this>m2 or this==m2 respectively
	 *         - a bool: true if this and m2 can be merged. If m12!=NULL, then
	 *                   *m12 has been correctly built.
	 *
	 * \warning if the bool in result is false, m12 can be partially built.
	 */
	std::pair<int,bool> cmp_and_add(const ExprMonomial& m2, ExprMonomial* m12, bool add) const;

	friend class ExprPolynomial;
	friend ExprMonomial operator*(const ExprMonomial& m1, const ExprMonomial& m2);
	friend ExprMonomial operator+(const ExprMonomial& m1, const ExprMonomial& m2);
	friend ExprMonomial operator-(const ExprMonomial& m1, const ExprMonomial& m2);
	friend ExprMonomial operator-(const ExprMonomial& m);
	friend int compare(const ExprMonomial& m1, const ExprMonomial& m2);
	friend std::ostream& operator<<(std::ostream&, const Term&);
	friend std::ostream& operator<<(std::ostream&, const ExprMonomial&);

	class ScalarTerm;
	class HalfCstDotProduct;
	class DotProduct;
	class CstMatrixTerm;
	class MatrixTerm;

	/*
	 * Scalar terms.
	 * Multiplication is commutative so terms
	 * are stored in a specific order (to ease simplification).
	 */
	std::list<Term*> terms;

	ExprCmp cmp;

	Dim _dim;
};

/**
 * Compare two monomials.
 */
int compare(const ExprMonomial& m1, const ExprMonomial& m2);

/**
 * Scalar multiplication
 * \throw  ExprMonomial::NullResult if the result is 0.
 */
ExprMonomial operator*(const Interval& c, const ExprMonomial& m);

/**
 * Monomial multiplication
 */
ExprMonomial operator*(const ExprMonomial& m1, const ExprMonomial& m2);

/**
 * Monomial addition
 * \warning - m1 and m2 must be fully factorizable.
 *            Ex: 2*x*C1*Y  and 3*x*C2*Y where C1 and C2 are two constant matrices
 *            gives: x*(2*C1+3*C2)Y (will throw ExprMonomial::NotAMonomial)
 * \throw  ExprMonomial::NullResult if the result is 0.
 */
ExprMonomial operator+(const ExprMonomial& m1, const ExprMonomial& m2);

/**
 * Monomial subtraction
 * \warning - m1 and m2 must be fully factorizable.
 *            Ex: 2*x*C1*Y  and 3*x*C2*Y where C1 and C2 are two constant matrices
 *            gives: x*(2*C1-3*C2)Y (will throw ExprMonomial::NotAMonomial)
 * \throw  ExprMonomial::NullResult if the result is 0.
 */
ExprMonomial operator-(const ExprMonomial& m1, const ExprMonomial& m2);

/**
 * Opposite monomial
 */
ExprMonomial operator-(const ExprMonomial& m1);

/**
 * Stream out a monomial
 */
std::ostream& operator<<(std::ostream&, const ExprMonomial::Term&);

/**
 * Stream out a monomial
 */
std::ostream& operator<<(std::ostream&, const ExprMonomial&);


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline int compare(const ExprMonomial& m1, const ExprMonomial& m2) {
	bool _ignore=false;
	return m1.cmp_and_add(m2,NULL,_ignore).first; // NULL: only compare
}

inline ExprMonomial operator*(const Interval& c, const ExprMonomial& m) {
	if (c.lb()==0 && c.ub()==0) throw ExprMonomial::NullResult();
	ExprMonomial m2(m);
	m2.coeff *=c;
	return m2;
}

} // end namespace ibex

#endif // __IBEX_EXPR_MONOMIAL_H__
