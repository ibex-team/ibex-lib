//============================================================================
//                                  I B E X
// File        : ibex_Certificate.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 21, 2017
// Last Update : Nov 13, 2018
//============================================================================

#ifndef __IBEX_CERTIFICATE_H__
#define __IBEX_CERTIFICATE_H__

#include "ibex_IntervalVector.h"
#include "ibex_VarSet.h"

namespace ibex {

class Certificate {
public:

	/**
	 * \brief Create a certificate.
	 */
	Certificate(const IntervalVector& box, const IntervalVector* unicity=NULL, const VarSet* varset=NULL);

	/**
	 * \brief Destructor.
	 */
	~Certificate();

	/**
	 * \brief Duplicate the certificate.
	 */
	Certificate(const Certificate& c);

	/**
	 * \brief Assignment.
	 */
	Certificate& operator=(const Certificate& c);

	/**
	 * \brief Existence box.
	 *
	 * If the status is INNER/BOUNDARY, represents the smallest box found
	 * enclosing the manifold.
	 * Otherwise, represents the "unknown" box.
	 */
	const IntervalVector& existence() const;

	/**
	 * \brief Unicity box.
	 *
	 * If the status is INNER/BOUNDARY, represents the largest superset
	 * of existence() found such that the solution enclosed is unique.
	 * Otherwise, represents the "unknown" box.
	 */
	const IntervalVector& unicity() const;

	/**
	 * \brief Variable/Parameter structure.
	 *
	 * Structure used to certify the box.
	 *
	 * This field is NULL in the following cases:
	 *   - the system is well-constrained (all are variables)
	 *   - the system has no equalities (all are parameters)
	 *   - the set of parameters has been fixed by the user
	 *     (same parameters for all solutions)
	 *   - the status is UNKNOWN or PENDING
	 */
	const VarSet* varset;

private:

	IntervalVector _existence;
	IntervalVector* _unicity; // NULL if none (<=> m=0)
};


/**
 * \brief Print the qualified box
 *
 * Print its status and the parameters/variables structure.
 */
std::ostream& operator<<(std::ostream& os, const Certificate& c);

/*============================================ inline implementation ============================================ */

inline Certificate::Certificate(const Certificate& c) :
		varset(c.varset? new VarSet(*c.varset) : NULL), _existence(c._existence),
		_unicity(c._unicity? new IntervalVector(*c._unicity) : NULL) {

}

inline Certificate& Certificate::operator=(const Certificate& c) {
	if (varset) delete varset;
	varset=c.varset? new VarSet(*c.varset) : NULL;
	_existence=c._existence;
	if (_unicity) delete _unicity;
	_unicity=c._unicity? new IntervalVector(*c._unicity) : NULL;
	return *this;
}

inline Certificate::~Certificate() {
	if (_unicity) delete _unicity;
	if (varset) delete varset;
}

inline const IntervalVector& Certificate::existence() const {
	return _existence;
}

inline const IntervalVector& Certificate::unicity() const {
	return _unicity? *_unicity : _existence;
}

} /* namespace ibex */

#endif /* __IBEX_CERTIFICATE_H__ */
