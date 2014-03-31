/* ============================================================================
 * I B E X - Constant Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 31, 2014
 * ---------------------------------------------------------------------------- */

#include "ibex_FncConstant.h"

namespace ibex {

void FncConstant::generate_comp() const {
	if (cst.dim.is_scalar()) {
		comp = new Fnc*[1];
		comp[0] = (Fnc*) this; // not modifiable anyway
	}

	Domain d(Dim::scalar());

	if (cst.dim.is_vector()) {
		int m=cst.dim.size();
		comp = new Fnc*[m];
		for (int i=0; i<m; i++) {
			d.i()=cst.v()[i];
			comp[i] = new FncConstant(d);
		}
	} else {
		int m=cst.dim.size();
		comp = new Fnc*[m];
		for (int i=0; i<cst.dim.dim2; i++)
			for (int j=0; j<cst.dim.dim3; j++) {
				d.i()=cst.m()[i][j];
				comp[i] = new FncConstant(d);
			}
	}
}

} // end namespace ibex
