/* ============================================================================
 * I B E X - ibex_Propagation.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcPropag.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Cell.h"
#include "ibex_Bsc.h"

using namespace std;

namespace ibex {

/*! Default propagation ratio. */
#define __IBEX_DEFAULT_RATIO_PROPAG           0.01

CtcPropag::CtcPropag(const Array<Ctc>& cl, double ratio, bool incremental) :
		  Ctc(cl[0].nb_var), list(cl), ratio(ratio), incremental(incremental),
		  accumulate(false), g(cl.size(), cl[0].nb_var), agenda(cl.size()),
		  all_vars(cl[0].nb_var) {

	for (int i=1; i<list.size(); i++)
		assert(list[i].nb_var==nb_var);

	for (int i=0; i<list.size(); i++)
		for (int j=0; j<nb_var; j++) {
			if (list[i].input[j]) g.add_arc(i,j,true);
			if (list[i].output[j]) g.add_arc(i,j,false);
		}

	all_vars.set_all();
//	cout << g << endl;
}

void CtcPropag::contract(IntervalVector& box) {
	contract(box,all_vars);
}

void  CtcPropag::contract(IntervalVector& box, const BoolMask& start) {

	assert(start.size()==nb_var);

	if (incremental) {
		for (int i=0; i<nb_var; i++) {
			if (start[i]) {
				set<int> ctrs=g.output_ctrs(i);
				for (set<int>::iterator c=ctrs.begin(); c!=ctrs.end(); c++)
					agenda.push(*c);
			}
		}
	} else { // push all the contractors
		for (int i=0; i<list.size(); i++)
			agenda.push(i);
	}

	int c; // current contractor

	/*
	 * old_box is either:
	 * - variables domains before last propagation ("fine" propagation, accumulate=true)
	 * - variables domains before last projection ("coarse" propagation, accumulate=false)
	 */
	IntervalVector old_box(box);

	//   VECTOR thres(_nb_var);        // threshold for propagation
	//   for (int i=1; i<=_nb_var; i++) {
	//     thres(i) = ratio*Diam(box(i));
	//     if (thres(i)<w) thres(i)=w;
	//   }
	//cout << "=========== Start propagation ==========" << endl;
	while (!agenda.empty()) {

		agenda.pop(c);

		set<int> vars=g.output_vars(c);

		// ===================== fine propagation =========================
		// reset the old box to the current domains just before contraction
		if (!accumulate) {
			for (set<int>::iterator v=vars.begin(); v!=vars.end(); v++) {
				old_box[*v] = box[*v];
			}
		}
		// ================================================================

		//cout << "Contraction with " << c << endl;

		try {
			list[c].contract(box);
		}
		catch (EmptyBoxException& e) {
			agenda.flush();
			//cout << "=========== End propagation ==========" << endl;
			//cout << "   empty!" << endl;
			throw e;
		}

		//cout << "  =>" << box[v] << endl;
		//cout << agenda << endl;

		for (set<int>::iterator it=vars.begin(); it!=vars.end(); it++) {
			int v=*it;
			//cout << "   " << old_box[v] << " % " << box[v] << "   " << old_box[v].ratiodelta(box[v]) << endl;
			//if (old_box[v].rel_distance(box[v])>=ratio) {
			if (old_box[v].ratiodelta(box[v])>=ratio) {
				set<int> ctrs=g.output_ctrs(v);
				for (set<int>::iterator c2=ctrs.begin(); c2!=ctrs.end(); c2++) {
					if (c!=*c2 || !list[c].idempotent())
						agenda.push(*c2);
				}
				// ===================== coarse propagation =========================
				// reset the old box to the current domains just after propagation
				if (accumulate)
					old_box[v] = box[v];
				// ================================================================
			}
		}
	}
	//cout << "=========== End propagation ==========" << endl;
	/* we cancel the "residual" contractions
	 * in order to trigger more propagation next
	 * time this function is called. This action
	 * is meaningful when the contract floor is
	 * small w.r.t the ratio here. */
	//   if (!reducted) box = propbox; // restore domains

}

const double CtcPropag::default_ratio = __IBEX_DEFAULT_RATIO_PROPAG;

} // namespace ibex
