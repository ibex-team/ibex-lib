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
#include "ibex_Cell.h"
#include "ibex_Bsc.h"

using namespace std;

namespace ibex {

CtcPropag::CtcPropag(const Array<Ctc>& cl, double ratio, bool incremental) :
		  Ctc(cl), list(cl), ratio(ratio), incremental(incremental),
		  accumulate(false), g(cl.size(), nb_var), agenda(cl.size()),
		  active(BitSet::empty(cl.size())) {

	assert(check_nb_var_ctc_list(cl));

	for (int i=0; i<list.size(); i++)
		for (int j=0; j<nb_var; j++) {
			if (list[i].input && (*list[i].input)[j]) g.add_arc(i,j,true);
			if (list[i].input && (*list[i].output)[j]) g.add_arc(i,j,false);
		}

	//cout << g << endl;
}

void CtcPropag::add_property(const IntervalVector& init_box, BoxProperties& map) {
	for (int i=0; i<list.size(); i++)
		list[i].add_property(init_box, map);
}

void CtcPropag::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcPropag::contract(IntervalVector& box, ContractContext& context) {

	assert(box.size()==nb_var);

	if (incremental) {
		/**
		 * Note: when context.impact() is NULL, we can
		 * also push all the contractors in a simple loop,
		 * as in the "else" part below.
		 * But the order in the agenda is different
		 * and this one turns to be slightly more efficient.
		 * Maybe we should do the same when incremental==false.
		 */

		for (int i=0; i<nb_var; i++) {
			if (context.impact[i]) {
				set<int> ctrs=g.output_ctrs(i);
				for (set<int>::iterator c=ctrs.begin(); c!=ctrs.end(); c++)
					agenda.push(*c);
			}
		}
	} else { // push all the contractors
		for (int i=0; i<list.size(); i++)
			agenda.push(i);
	}

	BitSet input_impact(context.impact); // save (useful?)

	/*
	 * Now, context.impact is the impact of a call to a
	 * subcontractor.
	 *
	 * When we call a contractor, we assume all
	 * its variables have been impacted although there might be
	 * only one of them impacted. In a future version, we may
	 * consider a more fine propagation where the information
	 * about the variables that are actually impacted is
	 * given to the awaken contractor.
	 */
	context.impact.fill(0,nb_var-1);

	// By default, all contractors are active
	active.fill(0,list.size()-1);


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

		context.output_flags.clear();

		list[c].contract(box, context);

		if (box.is_empty()) {
			agenda.flush();
			//cout << "=========== End propagation ==========" << endl;
			//cout << "   empty!" << endl;
			return;
		}

		if (context.output_flags[INACTIVE]) {
			active.remove(c);
		}

		for (set<int>::iterator it=vars.begin(); it!=vars.end(); it++) {
			int v=*it;
			//cout << "   " << old_box[v] << " % " << box[v] << "   " << old_box[v].ratiodelta(box[v]) << endl;
			//if (old_box[v].rel_distance(box[v])>=ratio) {
			if (old_box[v].ratiodelta(box[v])>=ratio) {
				set<int> ctrs=g.output_ctrs(v);
				for (set<int>::iterator c2=ctrs.begin(); c2!=ctrs.end(); c2++) {
					if ((c!=*c2 && active[*c2]) || (c==*c2 && !context.output_flags[FIXPOINT]))
						agenda.push(*c2);
				}
				// ===================== coarse propagation =========================
				// reset the old box to the current domains just after propagation
				if (accumulate)
					old_box[v] = box[v];
				// ================================================================
			}
		}

		//cout << "  =>" << box << endl;
		//cout << agenda << endl;

	}
	//cout << "=========== End propagation ==========" << endl;
	/* we cancel the "residual" contractions
	 * in order to trigger more propagation next
	 * time this function is called. This action
	 * is meaningful when the contract floor is
	 * small w.r.t the ratio here. */
	//   if (!reducted) box = propbox; // restore domains

	context.output_flags.clear(); // re-init

	if (active.empty())
		// TODO: does not respect the current definition of INACTIVE
		// which imposes that the contractor is inactive before contraction
		context.output_flags.add(INACTIVE);
}

} // namespace ibex
