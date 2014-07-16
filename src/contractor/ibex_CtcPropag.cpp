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
		  Ctc(cl), list(cl), ratio(ratio), incremental(incremental),
		  accumulate(false), g(cl.size(), nb_var), agenda(cl.size()),
		  _impact(nb_var), flags(Ctc::NB_OUTPUT_FLAGS), active(cl.size()) {

	assert(check_nb_var_ctc_list(cl));

	for (int i=0; i<list.size(); i++)
		for (int j=0; j<nb_var; j++) {
			if (list[i].input && (*list[i].input)[j]) g.add_arc(i,j,true);
			if (list[i].input && (*list[i].output)[j]) g.add_arc(i,j,false);
		}

//	cout << g << endl;
}


void  CtcPropag::contract(IntervalVector& box) {

	assert(box.size()==nb_var);

	/*
	 * When we call a contractor, we assume all
	 * its variables have been impacted although there might be
	 * only one of them impacted. In a future version, we may
	 * consider a more fine propagation where the information
	 * about the variables that are actually impacted is
	 * given to the awaken contractor.
	 */
	_impact.set_all();

	// By default, all contractors are active
	active.set_all();

	if (incremental) {
		/**
		 * impact() is the impact in input (given to CtcPropag).
		 * Not to be confused with _impact.
		 *
		 * Note: when impact() is NULL, we can
		 * also push all the contractors in a simple loop,
		 * as in the "else" part below.
		 * But the order in the agenda is different
		 * and this one turns to be slightly more efficient.
		 * Maybe we should do the same when incremental==false.
		 */

		assert(!impact() || (impact()->size()==nb_var));

		for (int i=0; i<nb_var; i++) {
			if (!impact() || (*impact())[i]) {
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
			list[c].contract(box, _impact, flags);
			if (flags[INACTIVE]) {
				active[c]=false;
			}
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
					if ((c!=*c2 && active[*c2]) || (c==*c2 && !flags[FIXPOINT]))
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
