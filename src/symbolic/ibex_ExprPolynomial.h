//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprPolynomial.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 30, 2020
// Last update : May 12, 2020
//============================================================================

#ifndef __IBEX_EXPR_POLYNOMIAL_H__
#define __IBEX_EXPR_POLYNOMIAL_H__

#include "ibex_ExprMonomial.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Expression polynomial.
 *
 */
class ExprPolynomial {
public:

	/**
	 * Build the null polynomial.
	 */
	ExprPolynomial(const Dim& dim=Dim::scalar());

	/**
	 * Build a constant (0-degree) polynomial.
	 */
	ExprPolynomial(const Interval& c);

	/**
	 * Build a constant (0-degree) matrix of polynomial.
	 */
	ExprPolynomial(const IntervalMatrix& m);

	/**
	 * Build a scalar/matrix monomial "e".
	 */
	explicit ExprPolynomial(const ExprNode& e);

	/**
	 * Build a single-monomial polynomial.
	 */
	ExprPolynomial(const ExprMonomial& p);

	/**
	 * Copy this.
	 */
	ExprPolynomial(const ExprPolynomial& p);

	/**
	 * Return the polynomial^2.
	 */
	ExprPolynomial square() const;

	/**
	 * Return the polynomial^2 (dynamic variant)
	 */
	ExprPolynomial* square_() const;

	/*
	 * Multiply the polynomial by c.
	 */
	ExprPolynomial& operator*=(const Interval& c);

	/*
	 * Multiply the polynomial by p.
	 */
	ExprPolynomial& operator*=(const ExprPolynomial& p);

	/*
	 * Add p to the polynomial
	 */
	ExprPolynomial& operator+=(const ExprPolynomial& p);

	/*
	 * Subtract p to the polynomial
	 */
	ExprPolynomial& operator-=(const ExprPolynomial& p);

	/**
	 * Count the total number of symbol occurrences.
	 */
	Matrix count_occ(const ExprOccCounter& c) const;

	/**
	 * True if this polynomial is a monomial,
	 * including a constant (as a zero-degree
	 * monomial).
	 * Ex:
	 *   3
	 *   xy^2
	 *   some constant matrix
	 */
	bool one_monomial() const;

	/**
	 * True if the polynom is a constant
	 */
	bool is_constant() const;

	/**
	 * String form
	 */
	operator std::string() const;

	/**
	 * Will create new nodes except for the
	 * original nodes (that plays the role
	 * of the polynomial variables).
	 *
	 * New nodes are pushed in the record
	 * vector to ease memory cleanup if
	 * provided. Otherwise: memory leaks!
	 *
	 * Note: Keeping track of nodes is complicated
	 * because simplification occurs when we
	 * build a polynomial and polynomials are
	 * built at different levels in Expr2Polynom
	 * (some expressions become terms of new
	 * polynomials that may simplified in turn).
	 */
	const ExprNode& to_expr(std::vector<const ExprNode*>* record=NULL) const;

	/**
	 * Sum of polynomials (dynamic variant)
	 */
	ExprPolynomial* add(const ExprPolynomial* p2) const;

	/**
	 * Subtraction of polynomials (dynamic variant)
	 */
	ExprPolynomial* sub(const ExprPolynomial* p2) const;

	/**
	 * Multiplication of a polynomial by a scalar (dynamic variant)
	 */
	ExprPolynomial* mul(const Interval& c) const;

	/**
	 * Multiplication of polynomials (dynamic variant)
	 */
	ExprPolynomial* mul(const ExprPolynomial* p2, ExprSimplify2* s) const;

	/*
	 * Minus operator (dynamic variant)
	 */
	ExprPolynomial* minus() const;

	/**
	 * Dimension
	 */
	Dim dim;

	std::list<ExprMonomial> mono;

protected:
	static ExprPolynomial zero(Dim d);

	static bool is_zero(const Interval& x);

	static bool is_zero(const IntervalMatrix& x);

	/**
	 * By copy?
	 *
	 * Add a monomial in increasing expression order.
	 *
	 * \precondition The monomial must be > all previously added
	 */
	void add_monomial(const ExprMonomial& mono);

	void add_or_sub(const ExprPolynomial& p2, bool add);

	/*
	 * Initialize an (empty) polynom to x*p
	 */
	ExprPolynomial& init_mult(const Interval& c, const ExprPolynomial& p2);

	/*
	 * Initialize an (empty) polynom to p1*p2
	 */
	ExprPolynomial& init_mult(const ExprPolynomial& p1, const ExprPolynomial& p2, ExprSimplify2* s);

	/*
	 * Initialize an (empty) polynom to p^2
	 */
	ExprPolynomial& init_square(const ExprPolynomial& p);

	void clear();

	friend ExprPolynomial operator+(const ExprPolynomial& p1, const ExprPolynomial& p2);

	friend ExprPolynomial operator-(const ExprPolynomial& p1, const ExprPolynomial& p2);

	friend ExprPolynomial operator*(const Interval& c, const ExprPolynomial& p2);

	friend ExprPolynomial operator*(const ExprPolynomial& p1, const ExprPolynomial& p2);
};

/**
 * Sum of polynomials
 */
ExprPolynomial operator+(const ExprPolynomial& p1, const ExprPolynomial& p2);

/**
 * Subtraction of polynomials
 */
ExprPolynomial operator-(const ExprPolynomial& p1, const ExprPolynomial& p2);

/**
 * Multiplication of a polynomial by a scalar
 */
ExprPolynomial operator*(const Interval& c, const ExprPolynomial& p2);

/**
 * Multiplication of polynomials
 */
ExprPolynomial operator*(const ExprPolynomial& p1, const ExprPolynomial& p2);

/*
 * Minus operator
 */
ExprPolynomial operator-(const ExprPolynomial& p1);

/**
 * Display a polynomial
 */
std::ostream& operator<<(std::ostream&, const ExprPolynomial&);

/**
 * Compare two polynomials
 */
int compare(const ExprPolynomial& p1, const ExprPolynomial& p2);

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline ExprPolynomial ExprPolynomial::zero(Dim d) {
	return ExprPolynomial(d);
}

inline void ExprPolynomial::add_monomial(const ExprMonomial& m) {
	assert(mono.empty() || compare(mono.back(),m)==-1);
	mono.push_back(m);
}

inline ExprPolynomial ExprPolynomial::square() const {
	return ExprPolynomial(dim).init_square(*this);
}

inline ExprPolynomial* ExprPolynomial::square_() const {
	return &((new ExprPolynomial(dim))->init_square(*this));
}

inline ExprPolynomial operator+(const ExprPolynomial& p1, const ExprPolynomial& p2) {
	return ExprPolynomial(p1)+=p2;
}

inline ExprPolynomial* ExprPolynomial::add(const ExprPolynomial* p2) const {
	return &((*new ExprPolynomial(*this))+=*p2);
}

inline ExprPolynomial operator-(const ExprPolynomial& p1, const ExprPolynomial& p2) {
	return ExprPolynomial(p1)-=p2;
}

inline ExprPolynomial* ExprPolynomial::sub(const ExprPolynomial* p2) const {
	return &((*new ExprPolynomial(*this))-=*p2);
}

inline ExprPolynomial operator*(const Interval& c, const ExprPolynomial& p2) {
	return ExprPolynomial(p2.dim).init_mult(c,p2);
}

inline ExprPolynomial* ExprPolynomial::mul(const Interval& c) const {
	return &((new ExprPolynomial(dim))->init_mult(c,*this));
}

inline ExprPolynomial operator*(const ExprPolynomial& p1, const ExprPolynomial& p2) {
	return ExprPolynomial(mul_dim(p1.dim,p2.dim)).init_mult(p1,p2,NULL);
}

inline ExprPolynomial* ExprPolynomial::mul(const ExprPolynomial* p2, ExprSimplify2* s) const {
	return &((new ExprPolynomial(mul_dim(dim,p2->dim)))->init_mult(*this,*p2,s));
}

inline ExprPolynomial operator-(const ExprPolynomial& p1) {
	return ExprPolynomial(p1)*=-1;
}

inline ExprPolynomial* ExprPolynomial::minus() const {
	return &((*new ExprPolynomial(*this))*=-1);
}

} // end namespace ibex

#endif // __IBEX_EXPR_POLYNOMIAL_H__
