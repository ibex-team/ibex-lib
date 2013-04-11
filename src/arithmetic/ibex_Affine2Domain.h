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

#include "ibex_Affine2MatrixArray.h"
#include "ibex_TemplateDomain.h"

namespace ibex {


/**
 * \ingroup arithmetic
 *
 * \brief Affine Domain.
 *
 * An affine domain is either:
 * <ul><li> an interval (#ibex:Affine2)
 *     <li> a vector of intervals (#ibex::Affine2Vector)
 *     <li> a matrix of intervals (#ibex::Affine2Matrix)
 *     <li> or an array of interval matrices (#ibex::Affine2MatrixArray)
 * </ul>
 *
 */
typedef TemplateDomain<Affine2> Affine2Domain;

template<>
inline TemplateDomain<Affine2>& TemplateDomain<Affine2>::operator&=(const TemplateDomain<Affine2>& d) {

	/* intersection is forbidden with affine forms */
	assert(false);
}

} // end namespace

#endif /* __IBEX_AFFINE2_DOMAIN_H__ */
