/* ============================================================================
 * I B E X - Affine2Domain definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : March 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE2_DOMAIN_H__
#define __IBEX_AFFINE2_DOMAIN_H__

#include <stdexcept>
#include "ibex_TemplateDomain.h"

namespace ibex {


/**
 * \ingroup arithmetic
 *
 * \brief Affine Domain.
 *
 * An affine domain is either:
 * <ul><li> an affine form (#ibex::Affine2)
 *     <li> a vector of affine forms (#ibex::Affine2Vector)
 *     <li> a matrix of affine forms (#ibex::Affine2Matrix)
 * </ul>
 *
 */
typedef TemplateDomain<Affine2>   Affine2Domain;
typedef TemplateDomain<AffineLin> AffineLinDomain;


template<>
inline TemplateDomain<Affine2>& TemplateDomain<Affine2>::operator&=(const TemplateDomain<Affine2>& ) {
	/* intersection is forbidden with affine forms */
        throw std::logic_error("intersection is forbidden with affine forms");
}


template<>
inline TemplateDomain<Affine2> atan2(const TemplateDomain<Affine2>& d1, const TemplateDomain<Affine2>& ) {
	/* atan2 is not implemented yet with affine forms */
	not_implemented("atan2 with affine forms");
	return d1;
}

template<>
inline TemplateDomain<Affine2> acosh(const TemplateDomain<Affine2>& d) {
	/* acosh is not implemented yet with affine forms */
	not_implemented("acosh with affine forms");
	return d;
}

template<>
inline TemplateDomain<Affine2> asinh(const TemplateDomain<Affine2>& d) {
	/* asinh is not implemented yet with affine forms */
	not_implemented("asinh with affine forms");
	return d;
}


template<>
inline TemplateDomain<Affine2> atanh(const TemplateDomain<Affine2>& d) {
	/* atanh is not implemented yet with affine forms */
	not_implemented("atanh with affine forms");
	return d;
}


template<>
inline TemplateDomain<AffineLin>& TemplateDomain<AffineLin>::operator&=(const TemplateDomain<AffineLin>& ) {
	/* intersection is forbidden with affine forms */
        throw std::logic_error("intersection is forbidden with affine forms");
}


template<>
inline TemplateDomain<AffineLin> atan2(const TemplateDomain<AffineLin>& d1, const TemplateDomain<AffineLin>& ) {
	/* atan2 is not implemented yet with affine forms */
	not_implemented("atan2 with affine forms");
	return d1;
}

template<>
inline TemplateDomain<AffineLin> acosh(const TemplateDomain<AffineLin>& d) {
	/* acosh is not implemented yet with affine forms */
	not_implemented("acosh with affine forms");
	return d;
}

template<>
inline TemplateDomain<AffineLin> asinh(const TemplateDomain<AffineLin>& d) {
	/* asinh is not implemented yet with affine forms */
	not_implemented("asinh with affine forms");
	return d;
}


template<>
inline TemplateDomain<AffineLin> atanh(const TemplateDomain<AffineLin>& d) {
	/* atanh is not implemented yet with affine forms */
	not_implemented("atanh with affine forms");
	return d;
}

} // end namespace

#endif /* __IBEX_AFFINE2_DOMAIN_H__ */
