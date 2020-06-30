//============================================================================
//                                  I B E X
// File        : ibex_ExprMonomial.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 30, 2020
// Last update : May 12, 2020
//============================================================================

#include "ibex_ExprMonomial.h"
#include "ibex_NodeMap.h"
#include "ibex_ExprSimplify2.h"

#include <sstream>

using namespace std;

namespace ibex {

class ExprMonomial::Term {
public:
	typedef enum { SCALAR, HALFCST_DOT, DOT, MATRIX, CST_MATRIX } TermType;

	Term(const Dim& dim, int power=1);

	Term(const Term&);

	virtual ~Term();

	/*
	 * Compare term expressions (but not powers) and set if they can
	 * be summable (note: this last info depends on the powers).
	 *
	 * -1 = less
	 *  0 = equal
	 * +1 = greater
	 *
	 * summable: true if the two terms can be added, ex:
	 *           (1,2)^T x + (3,4)^T x = (4,6)^T x.
	 *
	 * Note: There is systematically a const matrix inserted between 2 matrix nodes
	 * so that only sum of constant matrices have to be considered!
	 */
	virtual int compare_expr(const Term& t, bool& summable) const=0;

	int compare(const Term& t) const;

	virtual Term* copy() const=0;

	virtual Term* mult(const Term& t) const=0;

	virtual string to_string() const=0;

	virtual const ExprNode& to_expr(std::vector<const ExprNode*>* record) const=0;

	virtual TermType type() const=0;

	/*
	 * Total number of symbol occurrences for this term.
	 */
	virtual Matrix count_occ(const ExprOccCounter& c) const=0;

	Dim dim;

	int power;
};

int ExprMonomial::Term::compare(const ExprMonomial::Term& t) const {
	bool ignore;
	return compare_expr(t,ignore);
}

ostream& operator<<(ostream& os, const ExprMonomial::Term& t) {
	return os << t.to_string();
}

/*
 * A sclar sub-expression (which plays the role of a polynomial variable),
 * to some power.
 *
 */
class ExprMonomial::ScalarTerm : public ExprMonomial::Term {
public:

	/*
	 * Simple term e^power.
	 *
	 * Note: 'e' is never a constant expression
	 *       (see ExprSimplPolynom) although this
	 *       would make no problem here.
	 */
	ScalarTerm(const ExprNode& e, int power=1);

	/**
	 * Copy
	 */
	ScalarTerm(const ScalarTerm& t);

	virtual int compare_expr(const Term& t, bool& summable) const;

	virtual ScalarTerm* copy() const;

	virtual ScalarTerm* mult(const Term& t) const;

	virtual Term::TermType type() const;

	virtual Matrix count_occ(const ExprOccCounter& c) const;

	virtual string to_string() const;

	virtual const ExprNode& to_expr(std::vector<const ExprNode*>* record) const;

	/**
	 * Sub-expression ("symbol" of the polynomial)
	 */
	const ExprNode& e;

};

/*
 * Dot product between a constant vector and a vector expression.
 */
class ExprMonomial::HalfCstDotProduct : public ExprMonomial::Term {
public:
	/*
	 *  Precondition: e2 must be a column vector.
	 *
	 */
	HalfCstDotProduct(const IntervalVector& v1, const ExprNode& e2, int power);

	HalfCstDotProduct(const HalfCstDotProduct& h);

	virtual int compare_expr(const Term& t, bool& summable) const;

	virtual HalfCstDotProduct* copy() const;

	virtual HalfCstDotProduct* mult(const Term& t) const;

	virtual Term::TermType type() const;

	virtual Matrix count_occ(const ExprOccCounter& c) const;

	virtual string to_string() const;

	virtual const ExprNode& to_expr(std::vector<const ExprNode*>* record) const;

	const IntervalVector v1;

	const ExprNode& e2;
};

/*
 * Dot product between two vector expressions
 */
class ExprMonomial::DotProduct : public ExprMonomial::Term {
public:
	/**
	 * Dot product (e1*e2)^power
	 *
	 * Precondition: e1 must be two **column** vectors
	 * (for ordering purposes) and e1<e2
	 */
	DotProduct(const ExprNode& e1, const ExprNode& e2, int power);

	DotProduct(const DotProduct& d);

	virtual int compare_expr(const Term& t, bool& summable) const;

	virtual DotProduct* copy() const;

	virtual DotProduct* mult(const Term& t) const;

	virtual Term::TermType type() const;

	virtual string to_string() const;

	virtual const ExprNode& to_expr(std::vector<const ExprNode*>* record) const;

	virtual Matrix count_occ(const ExprOccCounter& c) const;

	const ExprNode& e1;

	const ExprNode& e2;
};

class ExprMonomial::MatrixTerm : public ExprMonomial::Term {
public:
	/*
	 * Note: there is systematically a const matrix term
	 *        between two non-const expression, e.g.:
	 *
	 *       A*((1,0);(0,1))*B
	 */
	MatrixTerm(const ExprNode& e);

	MatrixTerm(const MatrixTerm& t);

	virtual int compare_expr(const Term& t, bool& summable) const;

	virtual MatrixTerm* copy() const;

	virtual MatrixTerm* mult(const Term& t) const;

	virtual Term::TermType type() const;

	virtual Matrix count_occ(const ExprOccCounter& c) const;

	virtual string to_string() const;

	virtual const ExprNode& to_expr(std::vector<const ExprNode*>* record) const;

	/**
	 * Sub-expression ("symbol" of the polynomial)
	 */
	const ExprNode& e;
};

class ExprMonomial::CstMatrixTerm : public ExprMonomial::Term {
public:
	/*
	 * Note: there is systematically a const matrix term
	 *        between two non-const expression, e.g.:
	 *
	 *       A*((1,0);(0,1))*B
	 */
	CstMatrixTerm(const IntervalMatrix& m, bool is_identity);

	CstMatrixTerm(const CstMatrixTerm& t);

	virtual int compare_expr(const Term& t, bool& summable) const;

	virtual CstMatrixTerm* copy() const;

	virtual CstMatrixTerm* mult(const Term& t) const;

	virtual Term::TermType type() const;

	const IntervalMatrix m;

	virtual string to_string() const;

	virtual const ExprNode& to_expr(std::vector<const ExprNode*>* record) const;

	virtual Matrix count_occ(const ExprOccCounter& c) const;

	bool is_identity;
};

ExprMonomial::Term::Term(const Dim& dim, int power) : dim(dim), power(power) {

}

ExprMonomial::Term::Term(const ExprMonomial::Term& t) : dim(t.dim), power(t.power) {

}

ExprMonomial::Term::~Term() {

}

// ----------------------------------------------------------------------------------------

ExprMonomial::ScalarTerm::ScalarTerm(const ExprNode& e, int power) : Term(e.dim, power), e(e) {

}

ExprMonomial::ScalarTerm::ScalarTerm(const ExprMonomial::ScalarTerm& t) : Term(t), e(t.e) {

}

int ExprMonomial::ScalarTerm::compare_expr(const ExprMonomial::Term& t, bool& summable) const {
	summable=false; // by default
	if (t.type()!=ExprMonomial::Term::SCALAR) return -1;
	const ExprMonomial::ScalarTerm& t2=(const ExprMonomial::ScalarTerm&) t;
	int result;
	if ((result = ExprCmp().compare(e,t2.e))!=0) return result;
	else if (power==t.power) summable=true;
	return 0;
}

ExprMonomial::ScalarTerm* ExprMonomial::ScalarTerm::copy() const {
	return new ScalarTerm(*this);
}

ExprMonomial::ScalarTerm* ExprMonomial::ScalarTerm::mult(const ExprMonomial::Term& t) const {
	assert(t.type()==ExprMonomial::Term::SCALAR);
	const ExprMonomial::ScalarTerm& t2=((const ExprMonomial::ScalarTerm&) t);
	return new ExprMonomial::ScalarTerm(e, power+t2.power);
}

ExprMonomial::Term::TermType ExprMonomial::ScalarTerm::type() const {
	return SCALAR;
}

Matrix ExprMonomial::ScalarTerm::count_occ(const ExprOccCounter& c) const {
	return c.count(e);
}

string ExprMonomial::ScalarTerm::to_string() const {
	stringstream ss;
	ss << e;
	if (power!=1) ss << '^' << power;
	return ss.str();
}

const ExprNode& ExprMonomial::ScalarTerm::to_expr(std::vector<const ExprNode*>* record) const {
	const ExprNode* res = &e;
	if (power!=1) {
		res = & (pow(*res,power));
		if (record) record->push_back(res);
	}
	return *res;
}

// ----------------------------------------------------------------------------------------

ExprMonomial::HalfCstDotProduct::HalfCstDotProduct(const IntervalVector& v1, const ExprNode& e2, int power) :
				Term(Dim::scalar(), power), v1(v1), e2(e2) {

}

ExprMonomial::HalfCstDotProduct::HalfCstDotProduct(const ExprMonomial::HalfCstDotProduct& h) :
		Term(h), v1(h.v1), e2(h.e2) {

}

int ExprMonomial::HalfCstDotProduct::compare_expr(const ExprMonomial::Term& t, bool& summable) const {
	summable=false; // by default
	switch (t.type()) {
	case SCALAR: return 1;
	case HALFCST_DOT: break;
	case DOT: return -1;
	case CST_MATRIX: return -1;
	case MATRIX: return -1;
	}

	const ExprMonomial::HalfCstDotProduct& t2=(const ExprMonomial::HalfCstDotProduct&) t;
	int result;
	if ((result = ExprCmp().compare(e2,t2.e2))!=0) return result;
	else {
		// we can multiply if constants are different, ex:
		//         (1,2)'*x and (3,4)'*x  ---> (4,6)'*x
		// but not if they are to some powers.
		summable =  (power == t.power && ((v1==t2.v1) || (power==1)));
		return ExprCmp::compare(v1,t2.v1);
	}
}

ExprMonomial::HalfCstDotProduct* ExprMonomial::HalfCstDotProduct::copy() const {
	return new HalfCstDotProduct(*this);
}

ExprMonomial::HalfCstDotProduct* ExprMonomial::HalfCstDotProduct::mult(const ExprMonomial::Term& t) const {
	assert(t.type()==DotProduct::Term::HALFCST_DOT);
	const ExprMonomial::HalfCstDotProduct& t2=(const ExprMonomial::HalfCstDotProduct&) t;
	return new HalfCstDotProduct(v1, e2, power+t2.power);
}

ExprMonomial::Term::TermType ExprMonomial::HalfCstDotProduct::type() const {
	return HALFCST_DOT;
}

Matrix ExprMonomial::HalfCstDotProduct::count_occ(const ExprOccCounter& c) const {
	return c.count(e2);
}

string ExprMonomial::HalfCstDotProduct::to_string() const {
	stringstream ss;
	ss << '(' << v1 << "." << e2 << ')';
	if (power!=1) ss << "^" << power;
	return ss.str();
}

const ExprNode& ExprMonomial::HalfCstDotProduct::to_expr(std::vector<const ExprNode*>* record) const {
	const ExprNode* res = &(ExprConstant::new_vector(v1,true)*(e2));
	if (record) record->push_back(res);
	if (power!=1) {
		res = & (pow(*res,power));
		if (record) record->push_back(res);
	}
	return *res;
}

// ----------------------------------------------------------------------------------------

ExprMonomial::DotProduct::DotProduct(const ExprNode& e1, const ExprNode& e2, int power) :
		Term(Dim::scalar(), power), e1(e1), e2(e2) {

	assert(e1.dim.type()==Dim::COL_VECTOR);
	assert(e2.dim.type()==Dim::COL_VECTOR);
	assert(ExprCmp().compare(e1,e2)<=0);
}

ExprMonomial::DotProduct::DotProduct(const ExprMonomial::DotProduct& d) :
		Term(d), e1(d.e1), e2(d.e2) {

}

int ExprMonomial::DotProduct::compare_expr(const ExprMonomial::Term& t, bool& summable) const {
	summable=false; // by default
	switch (t.type()) {
	case SCALAR: return 1;
	case HALFCST_DOT: return 1;
	case DOT: break;
	case CST_MATRIX: return -1;
	case MATRIX: return -1;
	}

	ExprCmp cmp;
	const ExprMonomial::DotProduct& t2=(const ExprMonomial::DotProduct&) t;
	int result;
	if ((result =cmp.compare(e1,t2.e1))!=0) return result;
	else if ((result = cmp.compare(e2,t2.e2))!=0) return result;
	else {
		summable = (power==t2.power);
		return 0;
	}
}

ExprMonomial::DotProduct* ExprMonomial::DotProduct::copy() const {
	return new DotProduct(*this);
}

ExprMonomial::DotProduct* ExprMonomial::DotProduct::mult(const ExprMonomial::Term& t) const {
	assert(t.type()==DotProduct::Term::DOT);
	const ExprMonomial::DotProduct& t2=(const ExprMonomial::DotProduct&) t;
	assert(ExprCmp().compare(e1,t2.e1)==0);
	assert(ExprCmp().compare(e2,t2.e2)==0);
	DotProduct* d=new DotProduct(*this);
	d->power+=t2.power;
	return d;
}

ExprMonomial::Term::TermType ExprMonomial::DotProduct::type() const {
	return DOT;
}

Matrix ExprMonomial::DotProduct::count_occ(const ExprOccCounter& c) const {
	return c.occ_product(c.count(e1), c.count(e2));
}

string ExprMonomial::DotProduct::to_string() const {
	stringstream ss;
	ss << '(' << e1 << '.' <<  e2 << ')';
	if (power!=1) ss << "^" << power;
	return ss.str();
}

const ExprNode& ExprMonomial::DotProduct::to_expr(std::vector<const ExprNode*>* record) const {
	const ExprNode* res = &(e1*e2);
	if (record) record->push_back(res);
	if (power!=1) {
		res = & (pow(*res,power));
		if (record) record->push_back(res);
	}
	return *res;
}

// ----------------------------------------------------------------------------------------

ExprMonomial::MatrixTerm::MatrixTerm(const ExprNode& e) : Term(e.dim), e(e) {
}

ExprMonomial::MatrixTerm::MatrixTerm(const ExprMonomial::MatrixTerm& t) : Term(t), e(t.e) {
}

int ExprMonomial::MatrixTerm::compare_expr(const ExprMonomial::Term& t, bool& summable) const {
	summable=false;
	if (t.type()!=ExprMonomial::Term::MATRIX) return 1;
	const ExprMonomial::MatrixTerm& t2=(const ExprMonomial::MatrixTerm&) t;
	int result=ExprCmp().compare(e,t2.e);
	summable = (result==0);
	return result;
}

ExprMonomial::MatrixTerm* ExprMonomial::MatrixTerm::copy() const {
	return new MatrixTerm(*this);
}

ExprMonomial::MatrixTerm* ExprMonomial::MatrixTerm::mult(const ExprMonomial::Term& t) const {
	not_implemented("ExprMonomial: matrix power");
	return NULL;
}

ExprMonomial::Term::TermType ExprMonomial::MatrixTerm::type() const {
	return MATRIX;
}

Matrix ExprMonomial::MatrixTerm::count_occ(const ExprOccCounter& c) const {
	return c.count(e);
}

string ExprMonomial::MatrixTerm::to_string() const {
	stringstream ss;
	ss << e;
	return ss.str();
}

const ExprNode& ExprMonomial::MatrixTerm::to_expr(std::vector<const ExprNode*>* record) const {
	return e;
}

// ----------------------------------------------------------------------------------------

ExprMonomial::CstMatrixTerm::CstMatrixTerm(const IntervalMatrix& m, bool is_identity) :
		Term(Dim::matrix(m.nb_rows(),m.nb_cols())), m(m), is_identity(is_identity) {
}

ExprMonomial::CstMatrixTerm::CstMatrixTerm(const ExprMonomial::CstMatrixTerm& t) :
		Term(t), m(t.m), is_identity(t.is_identity) {
}

int ExprMonomial::CstMatrixTerm::compare_expr(const ExprMonomial::Term& t, bool& summable) const {
	switch (t.type()) {
	case SCALAR: return 1;
	case HALFCST_DOT: return 1;
	case DOT: return 1;
	case CST_MATRIX:
	{
		const CstMatrixTerm& t2=(const CstMatrixTerm&) t;
		if (t.dim==t2.dim) summable=true;
		if (is_identity && t2.is_identity) return 0; // faster
		else return ExprCmp::compare(m,t2.m);
	}
	break;
	default:
		// This case, in fact should never happen as there is systematically
		// a const matrix inserted between 2 matrix nodes.
		// Consequence: only sum of constant matrices have to be considered.
		//
		return -1;
	}
}

ExprMonomial::CstMatrixTerm* ExprMonomial::CstMatrixTerm::copy() const {
	return new CstMatrixTerm(*this);
}

ExprMonomial::CstMatrixTerm* ExprMonomial::CstMatrixTerm::mult(const ExprMonomial::Term& t) const {
	assert(t.type()==DotProduct::Term::CST_MATRIX);
	const ExprMonomial::CstMatrixTerm& t2=(const ExprMonomial::CstMatrixTerm&) t;
	assert(m.nb_cols()==t2.m.nb_rows());
	return new CstMatrixTerm(m*t2.m, is_identity);
}

ExprMonomial::Term::TermType ExprMonomial::CstMatrixTerm::type() const {
	return CST_MATRIX;
}

Matrix ExprMonomial::CstMatrixTerm::count_occ(const ExprOccCounter& c) const {
	return Matrix::zeros(m.nb_rows(), m.nb_cols());
}

string ExprMonomial::CstMatrixTerm::to_string() const {
	stringstream ss;
	if (is_identity) ss << "I"; // for debug
	else ss << m;
	return ss.str();
}

const ExprNode& ExprMonomial::CstMatrixTerm::to_expr(std::vector<const ExprNode*>* record) const {
	const ExprNode& expr = ExprConstant::new_matrix(m);
	if (record) record->push_back(&expr);
	return expr;
}

// ----------------------------------------------------------------------------------------

ExprMonomial::ExprMonomial() : coeff(Interval::one()) {

}

bool ExprMonomial::is_constant() const {
	return terms.empty() || terms.front()->type()==Term::CST_MATRIX;
}

ExprMonomial::ExprMonomial(const ExprNode& e, int power) : coeff(Interval::one()), _dim(e.dim) {
	assert(!dynamic_cast<const ExprConstant*>(&e)
			|| (dynamic_cast<const ExprConstant*>(&e)->is_mutable()));
	if (e.dim.is_scalar())
		terms.push_back(new ScalarTerm(e,power));
	else {
		if (power!=1) not_implemented("ExprMonomial: matrix powers M^k (please, replace by M*...*M");
		terms.push_back(new MatrixTerm(e));
	}
}

ExprMonomial::ExprMonomial(const Interval& x) : coeff(x), _dim(Dim::scalar()) {

}

ExprMonomial::ExprMonomial(const IntervalVector& x, bool in_row) : coeff(Interval::one()), _dim(in_row? Dim::row_vec(x.size()) : Dim::col_vec(x.size())) {
	if (x.size()==1)
		coeff *=x[0];
	else if (in_row) {
		IntervalMatrix m(1, x.size());
		m.set_row(0,x);
		terms.push_back(new CstMatrixTerm(m,false));
	} else {
		IntervalMatrix m(x.size(),1);
		m.set_col(0,x);
		terms.push_back(new CstMatrixTerm(m,false));
	}
}

ExprMonomial::ExprMonomial(const IntervalMatrix& x) : coeff(Interval::one()), _dim(Dim::matrix(x.nb_rows(), x.nb_cols())) {
	if (x.nb_cols()==1 && x.nb_rows()==1)
		coeff *=x[0][0];
	else
		terms.push_back(new CstMatrixTerm(x,false));
}

ExprMonomial::ExprMonomial(const ExprMonomial& m) : coeff(m.coeff), _dim(m.dim()) {
	for (list<ExprMonomial::Term*>::const_iterator it=m.terms.begin(); it!=m.terms.end(); ++it) {
		terms.push_back((*it)->copy());
	}
}

ExprMonomial::~ExprMonomial() {
	clear_terms();
}

void ExprMonomial::add_term(ExprMonomial::Term* t) {
	//if (!terms.empty()) cout  << "back=" << *terms.back() << " t="<< *t << " " << terms.back()->compare(*t) << endl;
	// order may not be respected only when a constant matrix is inserted after a matrix.
	assert(terms.empty() || (terms.back()->type()==Term::MATRIX && t->type()==Term::CST_MATRIX) || terms.back()->compare(*t)==-1);

	_dim = mul_dim(_dim, t->dim);

	terms.push_back(t);
}

void ExprMonomial::clear_terms() {
	for (list<ExprMonomial::Term*>::const_iterator it=terms.begin(); it!=terms.end(); ++it) {
		delete *it;
	}
	terms.clear();
}

void ExprMonomial::insert(ExprMonomial::Term* t) {
	list<ExprMonomial::Term*>::const_iterator it;
	int result = -1; // init
	for (it=terms.begin(); it!=terms.end() && result==-1; ++it) {
		result=(*it)->compare(*t);
	}
	if (result==0) {
		switch((*it)->type()) {
		case Term::SCALAR:      ((ScalarTerm*) *it)->power++; break;
		case Term::HALFCST_DOT: ((HalfCstDotProduct*) *it)->power++; break;
		case Term::DOT: 		((DotProduct*) *it)->power++; break;
		default:                terms.insert(it,t); break;
		}
	} else {
		assert(it==terms.end() || result==1);
		terms.insert(it,t);
	}
}

ExprMonomial ExprMonomial::square() const {
	ExprMonomial m;
	m.coeff = pow(coeff,2);
	list<Term*>::const_iterator it;
	for (it=terms.begin(); it!=terms.end() && (*it)->dim.is_scalar(); ++it) {
		m.add_term((*it)->mult(**it));
	}

	if (it==terms.end()) return m;

	list<Term*>::const_iterator first = it; // memorize first matrix term
	list<Term*>::const_iterator last = terms.end();
	--last;

	for (; it!=last; ++it) {
		m.add_term((*it)->copy());
	}

	if ((*last)->type()==ExprMonomial::Term::CST_MATRIX && (*first)->type()==ExprMonomial::Term::CST_MATRIX) {
		// **** constant matrix/matrix product ***************
		const ExprMonomial::CstMatrixTerm& t1=(const ExprMonomial::CstMatrixTerm&) **last;
		const ExprMonomial::CstMatrixTerm& t2=(const ExprMonomial::CstMatrixTerm&) **first;
		m.add_term(new ExprMonomial::CstMatrixTerm(t1.m*t2.m, t1.is_identity && t2.is_identity));
		++first;
	} else {
		m.add_term((*last)->copy());
		if ((*first)->type()!=ExprMonomial::Term::CST_MATRIX) {
			assert((*last)->dim.nb_cols()>1);
			// we must insert a constant matrix between the two terms
			m.add_term(new ExprMonomial::CstMatrixTerm(Matrix::eye((*last)->dim.nb_cols()),true));
		}
	}

	// The following loop duplicates the list of matrix terms, e.g.:
	//               (ABC)^2  ----> ABCABC
	// todo? we could also consider adding a new sub-expression (ABC)^2.
	for (it=first; it!=terms.end(); ++it) {
		m.add_term((*it)->copy());
	}
	return m;
}

pair<int,bool> ExprMonomial::cmp_and_add(const ExprMonomial& m2, ExprMonomial* m12, bool add) const {

	bool build=m12!=NULL;
	const ExprMonomial& m1(*this);

	if (&m1==&m2) { // to avoid mix with iterators
		if (build) {
			if (add) {
				// note: we don't have operator= to write *m12=2*m1 so we do it by hand
				m12->coeff = 2*m1.coeff;
				for (list<Term*>::const_iterator it=m1.terms.begin(); it!=m1.terms.end(); ++it)
					m12->add_term((*it)->copy());
			}
			// **WARNING* we make the assumption that the same ExprMonomial object
			// is never used to represent two different monomials. Otherwise, just like
			// for interval artihmetic x-x is not zero, the result should not be zero!
			// but (*this)+(-m2).
			else throw NullResult();
		}
		return make_pair(0,true);
	}

	ExprCmp cmp;

	//don't set the coefficient of m12 now !

	list<Term*>::const_iterator it1=m1.terms.begin();
	list<Term*>::const_iterator it2=m2.terms.begin();

	// we allow one term with constant parts that differ,
	// e.g.,
	// y*(1,2)^T*x and y*(2,3)^Tx  ===> y*(3,5)^Tx
	int result;
	int final_result=0;
	bool summable;

	while (it1!=m1.terms.end() || it2!=m2.terms.end()) {
		if (it1==m1.terms.end()) return make_pair(-1,false); // m1 is shorter so m1<m2 lexicographically
		if (it2==m2.terms.end()) return make_pair(1,false);  // m1 is longer so m1>m2 lexicographically

		//cout << "comparing " << **it1 << "  and  " << **it2;
		result = (*it1)->compare_expr(**it2, summable);
		//cout << " : " << result << endl;
		if (result!=0) {
			if (final_result!=0) return make_pair(final_result,false); // we can sum different terms only once.
			else if (!summable) return make_pair(result,false);
			else { // note: if factorizable==true: t1.power==t2.power==1
				final_result = result; // will be the return value if another "summable" case happens.
				if (build) {
					if ((*it1)->type()==Term::HALFCST_DOT) {
						const HalfCstDotProduct& t1=(const HalfCstDotProduct&) **it1;
						const HalfCstDotProduct& t2=(const HalfCstDotProduct&) **it2;
						IntervalVector v= add? m1.coeff*t1.v1 + m2.coeff*t2.v1 : m1.coeff*t1.v1 - m2.coeff*t2.v1;
						if (v.is_zero()) {
							m12->clear_terms();
							throw NullResult();
						} else
							m12->add_term(new HalfCstDotProduct(v, t1.e2, 1));
					} else {
						assert((*it1)->type()==Term::CST_MATRIX);
						const CstMatrixTerm& t1=(const CstMatrixTerm&) **it1;
						const CstMatrixTerm& t2=(const CstMatrixTerm&) **it2;
						IntervalMatrix m=add? m1.coeff*t1.m + m2.coeff*t2.m : m1.coeff*t1.m - m2.coeff*t2.m;
						if (m.is_zero()) {
							m12->clear_terms();
							throw NullResult();
						} else
							m12->add_term(new CstMatrixTerm(m,false));
					}
				}
			}
		} else {
			if ((*it1)->power<(*it2)->power) {
				++it1; // if there is no more terms in m1
				if (it1==m1.terms.end()) return make_pair(-1,false); // ex: x < x^2
				else return make_pair(1,false); // ex: xy < x^2
			} else if ((*it1)->power>(*it2)->power) {
				++it2; // if there is no more terms in m2
				if (it2==m2.terms.end()) return make_pair(1,false); // ex: x^2 > x
				else return make_pair(-1,false); // ex: x^2 < xy
			} else {
				if (build) m12->add_term((*it1)->copy());
			}
		}
		++it1; ++it2;
	}


	if (final_result==0) {
		final_result = ExprCmp::compare(m1.coeff, m2.coeff);
		if (build) {
			m12->coeff = add ? m1.coeff + m2.coeff : m1.coeff - m2.coeff;
			if (m12->coeff==0) {
				m12->clear_terms();
				throw NullResult();
			}
		}
	} else {
		if (build) m12->coeff = 1.0;
	}

	return make_pair(final_result,true);
}

ExprMonomial operator*(const ExprMonomial& m1, const ExprMonomial& m2) {
	return m1.mul(m2);
}

ExprMonomial ExprMonomial::mul(const ExprMonomial& m2, ExprSimplify2* s) const {
	if (this==&m2) return square(); // to avoid mix with iterators

	const ExprMonomial& m1=*this;
	ExprMonomial m;
	ExprCmp cmp;

	list<ExprMonomial::Term*>::const_iterator it1=m1.terms.begin();
	list<ExprMonomial::Term*>::const_iterator it2=m2.terms.begin();

	m.coeff = m1.coeff * m2.coeff;

	while (it1!=m1.terms.end() && (*it1)->dim.is_scalar() &&
		   it2!=m2.terms.end() && (*it2)->dim.is_scalar()) {

		// note: we cannot factor dot products with different constant vectors,
		// like c1^Tx and c2^Tx ---> gives (c1^Tx)*(c2^Tx)

		bool ignore;
		int cc=(*it1)->compare_expr(**it2, ignore);

		switch (cc) {
		case -1:
			m.add_term((*it1)->copy());
			++it1;
			break;
		case +1:
			m.add_term((*it2)->copy());
			++it2;
			break;
		case 0:
			m.add_term((*it1)->mult(**it2));
			++it1;
			++it2;
			break;
		default:
			ibex_error("[ExprMonomial] bad comparison value");
		}
	}
	for (; it1!=m1.terms.end() && (*it1)->dim.is_scalar(); ++it1) {
		m.add_term((*it1)->copy());
	}
	for (; it2!=m2.terms.end() && (*it2)->dim.is_scalar(); ++it2) {
		m.add_term((*it2)->copy());
	}

	// last matrix term of m1 (end() if none)
	list<ExprMonomial::Term*>::const_iterator last1=m1.terms.end();
	if (it1!=m1.terms.end()) {
		--last1;
		assert(!(*last1)->dim.is_scalar());
	}

	// it2 is now the first matrix term of m2
	if (last1!=m1.terms.end() && it2!=m2.terms.end()) {

		if ((*last1)->dim.nb_rows()==1 && (*it2)->dim.nb_cols()==1) {

			// ******** Case when a dot product appears ******
			// we first deal with the possibility to have a dot product
			// appearing when multiplying m1 and m2, ex:
			// ex:    m1= ...*v  and m2=u*...  where v=row vector and u=col vector
			if ((*last1)->type()==ExprMonomial::Term::CST_MATRIX) {
				const ExprMonomial::CstMatrixTerm& t1=(const ExprMonomial::CstMatrixTerm&) **last1;
				if ((*it2)->type()==ExprMonomial::Term::CST_MATRIX) {
					const ExprMonomial::CstMatrixTerm& t2=(const ExprMonomial::CstMatrixTerm&) **it2;
					m.coeff *= t1.m.row(0)*t2.m.col(0);
				} else {
					const ExprMonomial::MatrixTerm& t2=(const ExprMonomial::MatrixTerm&) **it2;
					m.insert(new ExprMonomial::HalfCstDotProduct(t1.m.row(0), t2.e, 1));
				}
			} else if ((*it2)->type()==ExprMonomial::Term::CST_MATRIX) {
				const ExprMonomial::CstMatrixTerm& t2=(const ExprMonomial::CstMatrixTerm&) **it2;
				const ExprMonomial::MatrixTerm& t1=(const ExprMonomial::MatrixTerm&) **last1;
				// note: only systematic simplification (no recursive call to Expr2Polynom)
				const ExprNode& e1T = s? *s->visit(s->rec(transpose(t1.e))) :
						(const ExprNode&) transpose(t1.e); // no simplification / memory leak
				m.insert(new ExprMonomial::HalfCstDotProduct(t2.m.col(0), e1T, 1));
			} else {
				const ExprMonomial::MatrixTerm& t1=(const ExprMonomial::MatrixTerm&) **last1;
				const ExprMonomial::MatrixTerm& t2=(const ExprMonomial::MatrixTerm&) **it2;
				const ExprNode& e1T= s? *s->visit(s->rec(transpose(t1.e))) :
						(const ExprNode&) transpose(t1.e); // no simplification / memory leak
				if (cmp.compare(e1T,t2.e)<0)
					m.insert(new ExprMonomial::DotProduct(e1T, t2.e, 1));
				else
					m.insert(new ExprMonomial::DotProduct(t2.e, e1T, 1));
			}
			if (last1==it1) { // it1 is the first matrix term.
				// in this case, there is nothing left to do with m1
				// (we avoid this way to handle the situation last1<it1 !)
				it1=last1=m1.terms.end();
			} else
				--last1;
			++it2;
		}
	}

	for (; it1!=last1; ++it1) {
		m.add_term((*it1)->copy());
	}

	if (it1!=m1.terms.end()) {
		if (it2!=m2.terms.end()) {
			if ((*it1)->type()==ExprMonomial::Term::CST_MATRIX) {
				if ((*it2)->type()==ExprMonomial::Term::CST_MATRIX) {
					// **** constant matrix/matrix product ***************
					const ExprMonomial::CstMatrixTerm& t1=(const ExprMonomial::CstMatrixTerm&) **it1;
					const ExprMonomial::CstMatrixTerm& t2=(const ExprMonomial::CstMatrixTerm&) **it2;
					m.add_term(new ExprMonomial::CstMatrixTerm(t1.m*t2.m, t1.is_identity && t2.is_identity));
				} else {
					m.add_term((*it1)->copy());
					m.add_term((*it2)->copy());
				}
			} else {
				m.add_term((*it1)->copy());

				if ((*it2)->type()!=ExprMonomial::Term::CST_MATRIX) {
					// we must insert an identity matrix between the two terms
					// unless the dimension is 1
					if ((*it1)->dim.nb_cols()>1)
						m.add_term(new ExprMonomial::CstMatrixTerm(Matrix::eye((*it1)->dim.nb_cols()),true));
				}
				m.add_term((*it2)->copy());
			}
			++it1;
			++it2;
		} else {
			// simply insert the last term of m1
			m.add_term((*it1)->copy());
			++it1;
		}
	}

	// note we may have it1!=m1.terms.end() here if
	// the "dot product" simplification occurred above.
	for (; it2!=m2.terms.end(); ++it2) {
		m.add_term((*it2)->copy());
	}

	return m;
}

ExprMonomial operator+(const ExprMonomial& m1, const ExprMonomial& m2) {
	ExprMonomial m12;
	bool factor = m1.cmp_and_add(m2,&m12,true).second; // may throw NullResult
	if (!factor) throw ExprMonomial::NotAMonomial();
	return m12;
}

ExprMonomial operator-(const ExprMonomial& m1, const ExprMonomial& m2) {
	ExprMonomial m12;
	bool factor = m1.cmp_and_add(m2,&m12,false).second; // may throw NullResult
	if (!factor) throw ExprMonomial::NotAMonomial();
	return m12;
}

ExprMonomial operator-(const ExprMonomial& m1) {
	ExprMonomial m(m1); // deep copy
	m.coeff = -m.coeff;
	return m;
}

ExprMonomial::operator string() const {
	stringstream ss;
	if (coeff.is_degenerated()) {
		if (coeff.lb()!=1 || coeff.ub()!=1) {
			if (coeff.lb()!=-1 || coeff.ub()!=-1)
				ss << coeff.lb();
			else
				if (terms.empty())
					ss << "-1";
				else
					ss << '-';
		} else if (terms.empty())
			ss << '1';
		else ; // nothing
	}
	else ss << coeff;

	for (list<ExprMonomial::Term*>::const_iterator it=terms.begin(); it!=terms.end(); ++it) {
		ss << **it;
	}
	return ss.str();
}

const ExprNode& ExprMonomial::to_expr(std::vector<const ExprNode*>* record) const {
	const ExprNode* e = NULL;
	for (list<Term*>::const_iterator it=terms.begin();  it!=terms.end(); ++it) {
		if ((*it)->type()==Term::CST_MATRIX && ((CstMatrixTerm*) *it)->is_identity) continue;
		if (e) {
			e = & (*e * (*it)->to_expr(record));
			if (record) record->push_back(e);
		} else
			e = & (*it)->to_expr(record);
	}
	if (!e) {
		e = & ExprConstant::new_scalar(coeff);
		if (record) record->push_back(e);
	}
	else
		if (coeff.lb()!=1 || coeff.ub()!=1) {
			if (coeff.lb()==-1 && coeff.ub()==-1) {
				e = & (-*e);
				if (record) record->push_back(e);
			} else {
				const ExprConstant& c=ExprConstant::new_scalar(coeff);
				if (record) record->push_back(&c);
				e = & (c * (*e));
				if (record) record->push_back(e);
			}
		}
	return *e;
}

Matrix ExprMonomial::count_occ(const ExprOccCounter& c) const {
	Matrix m=Matrix::zeros(1,1);
	for (list<Term*>::const_iterator it=terms.begin(); it!=terms.end(); ++it) {
		m=c.occ_product(m, (*it)->count_occ(c));
	}
	return m;
}

Dim ExprMonomial::dim() const {
	return _dim;
}

ostream& operator<<(ostream& os, const ExprMonomial& m) {
	return os << ((string) m);
}

} // end namespace ibex;
