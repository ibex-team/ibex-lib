//============================================================================
//                                  I B E X                                   
// File        : ibex_Paver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#include "ibex_Paver.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

Paver::Paver(const Array<Ctc>& c, Bsc& b) :
		Strategy(b), ctc(c), capacity(-1), ctc_loop(true),
		paving(new SubPaving[c.size()]) {

}

Paver::Paver(const Array<Ctc>& c, Bsc& b, CellBuffer& buffer) :
		Strategy(b,buffer), ctc(c),
		capacity(-1), ctc_loop(true), paving(new SubPaving[c.size()]) {
}

void Paver::contract(Cell& cell) {
	int i=0; // contractor number

	int n=ctc.size(); // number of contractors

	// count the number of contractors in a row
	// that could not modify the box. When fix_count=n,
	// fixpoint is reached.
	// In a future version, we may use an AC3 loop instead.
	int fix_count=0;

	// used to compare boxes before and after contraction
	IntervalVector tmpbox(cell.box.size());

	try {
		while (fix_count<n) {

			//cout << "[contractor " << i << "] box=" << endl;

			// 	  for (int j=1; j<=box.size(); j++) {
			// 	    cout.precision(17);
			// 	    cout << "box[" << j << "]=" << box[j] << endl;
			// 	  }
			if (trace)  cout << "    ctc " << i;
			tmpbox=cell.box;

			ctc[i].contract(cell.box);

			if (tmpbox.rel_distance(cell.box)>0) {
				fix_count=0;

				check_capacity();
				paving[i].add(tmpbox,cell.box);
				paving_size++;

				if (trace) cout << " -> contracts" << endl;

			} else {
				fix_count++;
				if (trace) cout << " -> nothing" << endl;
			}

			i = (i+1)%ctc.size();

		}
	} catch(EmptyBoxException&) {
		assert(cell.box.is_empty());
		if (trace) cout << " -> empty set" << endl;

		check_capacity();
		paving[i].add(tmpbox);
		paving_size++;
	}

}

void Paver::pave(const IntervalVector& init_box) {

	assert(ctc.size()>0);

	start(init_box);

	for (int i=0; i<ctc.size(); i++) {
		ctc[i].init_root(*buffer.top());
	}

	Cell* c;

	while ((c=next_cell())) {
		contract(*c);
	}
}

} // end namespace ibex
