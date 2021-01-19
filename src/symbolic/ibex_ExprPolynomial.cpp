//============================================================================
//                                  I B E X
// File        : ibex_ExprPolynomial.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 30, 2020
// Last update : May 12, 2020
//============================================================================

#include "ibex_ExprPolynomial.h"
#include "ibex_ExprCmp.h"

#include <sstream>
#include <numeric>

using namespace std;

namespace ibex {

bool ExprPolynomial::is_zero(const Interval& x) {
	return (x.lb()==0 && x.ub()==0);
}

bool ExprPolynomial::is_zero(const IntervalMatrix& x) {
	return x==Matrix::zeros(x.nb_rows(), x.nb_cols());
}

bool ExprPolynomial::one_monomial() const {
	return mono.empty() || mono.size()==1;
}

bool ExprPolynomial::is_constant() const {
	return mono.empty() || (mono.size()==1 && mono.front().is_constant());
}

ExprPolynomial::ExprPolynomial(const Dim& d) : dim(d) {
}

ExprPolynomial::ExprPolynomial(const IntervalMatrix& m) : dim(Dim::matrix(m.nb_rows(), m.nb_cols())) {
	if (!is_zero(m))
		mono.emplace_back(m);
}

ExprPolynomial::ExprPolynomial(const Interval& c) : dim(Dim::scalar()) {
	if (!is_zero(c)) {
		mono.emplace_back(c);
	}
}

ExprPolynomial::ExprPolynomial(const ExprNode& e) : ExprPolynomial(e.dim) {
	mono.emplace_back(e);
}

ExprPolynomial::ExprPolynomial(const ExprMonomial& e) : ExprPolynomial(e.dim()) {
	mono.push_back(e);
}

ExprPolynomial::ExprPolynomial(const ExprPolynomial& p) : dim(p.dim), mono(p.mono) {

}

void ExprPolynomial::clear() {
	mono.clear();
}

ExprPolynomial& ExprPolynomial::operator*=(const Interval& c) {
	if (is_zero(c))  {
		clear();
	} else {
		for (list<ExprMonomial>::iterator it=mono.begin(); it!=mono.end(); ++it) {
			it->coeff *= c;
		}
	}
	return *this;
}

void ExprPolynomial::add_or_sub(const ExprPolynomial& p2, bool add) {

	assert(dim==p2.dim);

	if (this==&p2) {
		if (add) (*this)*=2; // to avoid mix with iterators
		else {
			clear();
		}
		return;
	}

	list<ExprMonomial>::const_iterator it1=   mono.begin();
	list<ExprMonomial>::const_iterator it2=p2.mono.begin();

	while (it1!=mono.end() && it2!=p2.mono.end()) {

		ExprMonomial m12;
		try {
			//cout<< "try to sum " << *it1 << " and " << *it2 << endl;
			pair<int,bool> result = it1->cmp_and_add(*it2, &m12, add);

			if (result.second) {
				// remove old monomial...
				it1 = mono.erase(it1);
				// ...and replace it by m12.
				// The order between monomials is preserved
				// by factorization (since symbols and powers do
				// not change)
				mono.insert(it1, m12);
				++it2;
			} else if (result.first==-1) {
				++it1; // already present, skip
			} else {
				assert(result.first==1);
				mono.insert(it1, add? *it2 : -(*it2));
				++it2;
			}
		} catch(ExprMonomial::NullResult&) {
			it1 = mono.erase(it1);
			++it2;
		}
	}
	for (; it2!=p2.mono.end(); ++it2) add_monomial(add? (*it2) : -(*it2));
}

ExprPolynomial& ExprPolynomial::operator+=(const ExprPolynomial& p) {
	add_or_sub(p,true);
	return *this;
}

ExprPolynomial& ExprPolynomial::operator-=(const ExprPolynomial& p) {
	add_or_sub(p,false);
	return *this;
}

ExprPolynomial& ExprPolynomial::init_square(const ExprPolynomial& p) {
	assert(dim.nb_rows()==dim.nb_cols());
	assert(dim == p.dim);

	for (list<ExprMonomial>::const_iterator it1=p.mono.begin(); it1!=p.mono.end(); ++it1) {
		for (list<ExprMonomial>::const_iterator it2=it1; it2!=p.mono.end(); ++it2) {
			// order or monomials is not necessarily preserved, ex: (x(1,2)+(1,2))*(x(1;2)+(1;2)) -> 5 + ...
			if (it1==it2)
				(*this) +=(it1->square());
			else
				(*this) +=(2*(*it1)*(*it2));
		}
	}
	return *this;
}

ExprPolynomial& ExprPolynomial::init_mult(const Interval& c, const ExprPolynomial& p) {
	assert(dim==p.dim);

	if (is_zero(c)) return *this; // <=>zero

	for (list<ExprMonomial>::const_iterator it=p.mono.begin(); it!=p.mono.end(); ++it) {
		add_monomial(c*(*it));
	}
	return *this;
}

ExprPolynomial& ExprPolynomial::init_mult(const ExprPolynomial& p1, const ExprPolynomial& p2, ExprSimplify2* s) {
	assert(dim==mul_dim(p1.dim,p2.dim));
	if (&p1==&p2) return init_square(p1); // to avoid mix with iterators

	for (list<ExprMonomial>::const_iterator it1=p1.mono.begin(); it1!=p1.mono.end(); ++it1) {
		for (list<ExprMonomial>::const_iterator it2=p2.mono.begin(); it2!=p2.mono.end(); ++it2) {
			// order or monomials is not necessarily preserved, ex: (x(1,2)+(1,2))*(y(1;2)+(1;2)) -> 5 + ...
			(*this) += it1->mul(*it2,s);
		}
	}
	return *this;
}

ExprPolynomial::operator string() const {
	stringstream ss;
	if (mono.empty())
		ss << "0";
	else
		for (list<ExprMonomial>::const_iterator it=mono.begin(); it!=mono.end(); ++it) {
			if (it!=mono.begin()) ss << '+';
			ss << *it;
		}
	return ss.str();
}

const ExprNode& ExprPolynomial::to_expr(std::vector<const ExprNode*>* record) const {
	const ExprNode* this_expr;
	if (mono.empty()) {
		this_expr = &ExprConstant::new_matrix(Matrix::zeros(dim.nb_rows(), dim.nb_cols()));
		if (record) record->push_back(this_expr);
	}
	else
		for (list<ExprMonomial>::const_iterator it=mono.begin(); it!=mono.end(); ++it) {
			if (it==mono.begin())
				this_expr = &it->to_expr(record);
			else {
				if (it->coeff.ub()<0) {
					// trick to replace x+(-y) by x-y
					(Interval&) it->coeff = -it->coeff;
					this_expr = &(*this_expr - it->to_expr(record));
					(Interval&) it->coeff = -it->coeff;
				} else
					this_expr = &(*this_expr + it->to_expr(record));
				if (record) record->push_back(this_expr);
			}
		}
	return *this_expr;
}

Matrix ExprPolynomial::count_occ(const ExprOccCounter& c) const {
	// could we use std::accumulate with Matrix?
	if (mono.empty()) return Matrix::zeros(dim.nb_rows(), dim.nb_cols());
	list<ExprMonomial>::const_iterator it=mono.begin();
	Matrix res=mono.begin()->count_occ(c);
	for (; it!=mono.end(); ++it) res += it->count_occ(c);
	return res;
}

ostream& operator<<(ostream& os, const ExprPolynomial& p) {
	return os << (string) p;
}

int compare(const ExprPolynomial& p1, const ExprPolynomial& p2) {
	list<ExprMonomial>::const_iterator it1=p1.mono.begin();
	list<ExprMonomial>::const_iterator it2=p2.mono.begin();

	int result;
	while (it1!=p1.mono.end() && it2!=p2.mono.end()) {

		if ((result = compare(*it1,*it2))!=0) return result;
		++it1;
		++it2;
	}

	if (it1!=p1.mono.end()) return 1;
	else if (it2!=p2.mono.end()) return -1;
	else return 0;
}

} // end namespace ibex;
