/* ============================================================================
 * I B E X - AffineDomain definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : March 08, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE_DOMAIN_H__
#define __IBEX_AFFINE_DOMAIN_H__

#include <stdexcept>
#include "ibex_Affine.h"
#include "ibex_AffineVector.h"
#include "ibex_AffineMatrix.h"

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
typedef TemplateDomain<Affine2> Affine2Domain;
typedef TemplateDomain<Affine3> Affine3Domain;


/*
template<class T>
Array< TemplateDomain< AffineMain<T> > > convert_to_affinedomain(const Array<Domain>& d) {
	const Array<const Domain>* d2 =(const Array<const Domain>*) (&d);
	return convert_to_affinedomain<T>(*d2);
}


template<class T>
Array< TemplateDomain< AffineMain<T> > > convert_to_affinedomain(const Array<const Domain>& d) {
	Array< TemplateDomain< AffineMain<T> > > out(d.size());

	int n=0;
	for (int i =0; i<=d.size(); i++) {
		n += d[i].dim.size();
	}

	int var = 1;
	for (int i =0; i<=d.size(); i++) {

		switch (d[i].dim.type()) {
		case Dim::SCALAR: {
			AffineMain<T> tmp(n, var, d[i].i());
			out.set_ref(i, *new TemplateDomain< AffineMain<T> >(tmp));
			var++;
			break;
		}
		case Dim::ROW_VECTOR: {
			AffineMainVector<T> vec(d[i].dim.size());
			for (int k = 0; k < d[i].dim.size(); k++) {
				vec[k] = AffineMain<T>(n, var, (d[i].v())[k]);
				var++;
			}

			out.set_ref(i, *new TemplateDomain< AffineMain<T> >(vec,true));
			break;
		}
		case Dim::COL_VECTOR: {
			AffineMainVector<T> vec(d[i].dim.size());
			for (int k = 0; k < d[i].dim.size(); k++) {
				vec[k] = AffineMain<T>(n, var, (d[i].v())[k]);
				var++;
			}
			out.set_ref(i, *new TemplateDomain< AffineMain<T> >(vec,false));
			break;
		}
		case Dim::MATRIX: {
			AffineMainMatrix<T> mat(d[i].dim.nb_rows(),d[i].dim.nb_cols());
			for (int k = 0; k < d[i].dim.nb_rows(); k++) {
				for (int p = 0; p < d[i].dim.nb_cols(); p++) {
					mat[k][p] = AffineMain<T>(n, var, (d[i].m())[k][p]);
					var++;
				}
			}
			out.set_ref(i, *new TemplateDomain< AffineMain<T> >(mat));
			break;
		}
		}
	}

	return out;

}
*/


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
inline TemplateDomain<Affine3>& TemplateDomain<Affine3>::operator&=(const TemplateDomain<Affine3>& ) {
	/* intersection is forbidden with affine forms */
        throw std::logic_error("intersection is forbidden with affine forms");
}


template<>
inline TemplateDomain<Affine3> atan2(const TemplateDomain<Affine3>& d1, const TemplateDomain<Affine3>& ) {
	/* atan2 is not implemented yet with affine forms */
	not_implemented("atan2 with affine forms");
	return d1;
}

template<>
inline TemplateDomain<Affine3> acosh(const TemplateDomain<Affine3>& d) {
	/* acosh is not implemented yet with affine forms */
	not_implemented("acosh with affine forms");
	return d;
}

template<>
inline TemplateDomain<Affine3> asinh(const TemplateDomain<Affine3>& d) {
	/* asinh is not implemented yet with affine forms */
	not_implemented("asinh with affine forms");
	return d;
}


template<>
inline TemplateDomain<Affine3> atanh(const TemplateDomain<Affine3>& d) {
	/* atanh is not implemented yet with affine forms */
	not_implemented("atanh with affine forms");
	return d;
}

} // end namespace

#endif /* __IBEX_AFFINE_DOMAIN_H__ */
