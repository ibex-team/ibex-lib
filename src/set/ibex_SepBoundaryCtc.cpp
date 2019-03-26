//============================================================================
//                                  I B E X
// File        : ibex_SepBoundaryCtc.h
// Author      : Gilles Chabert, Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include "ibex_SepBoundaryCtc.h"
#include <cassert>

namespace ibex {

SepBoundaryCtc::SepBoundaryCtc(Ctc& _ctc_boundary, Pdc& _is_inside) : Sep(_is_inside.nb_var), ctc_boundary(_ctc_boundary), is_inside(_is_inside) {

}

void SepBoundaryCtc::separate(IntervalVector& x_in, IntervalVector& x_out) {

	assert(x_in==x_out);

	IntervalVector box = x_in;
	IntervalVector box0 = box;

	ctc_boundary.contract(box);

	// [gch] not sure the following line is necessary.
	// If  box==box0 the diff will simply return a
	// zero-sized array

	if (box==box0) return;     // nothing contracted.

	x_in = box; x_out = box;

	IntervalVector* rest;
	int n=box0.diff(box,rest, false); // calculate the set difference

	BoolInterval res;

	/**
	 * TODO
	 *
	 * [gch] I think the following loop could be improved by
	 * taking benefit of the continuity of the set boundary:
	 * all contiguous boxes in the difference are on the same
	 * side (inner our outer) so that it is useless to call
	 * for every one the is_inside test.
	 *
	 */
	for(int i = 0; i < n; i++) {
		// try to classify the box using first the midpoint:
		Vector candidate_pt=rest[i].mid();

		// or using random points NB_SAMPLES times
        for (int k=0; k<NB_SAMPLES; k++) {

			res = is_inside.test(candidate_pt);

			if (res==YES) {
				x_out |= rest[i];
				break;
			}
			else if (res==NO) {
				x_in |= rest[i]; break;
			} else {
				x_in |=rest[i];
				x_out |= rest[i];
				break;
			}

			candidate_pt = rest[i].random();
		}
	}
	delete[] rest;
}

} // namespace ibex
