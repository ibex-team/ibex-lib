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

#include "ibex_Propagation.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

/*! Default propagation ratio. */
#define __IBEX_DEFAULT_RATIO_PROPAG           0.1

Propagation::Propagation(const Array<Contractor>& cl, double ratio, bool incremental) :
		  Contractor(cl[0].nb_var), list(cl), ratio(ratio), incremental(incremental),
		  g(cl.size(), cl[0].nb_var), agenda(cl.size(), cl[0].nb_var) {

	for (int i=1; i<list.size(); i++)
		assert(list[i].nb_var==nb_var);

	for (int i=0; i<list.size(); i++)
		for (int j=0; j<nb_var; j++)
			if (list[i].can_contract(j)) {
				g.add_arc(i,j,1);
			}

}

void Propagation::contract(IntervalVector& box) {
	contract(box, Indicators(box.size()));
}

void  Propagation::contract(IntervalVector& _box, const Indicators& indic) {

	if (incremental && indic.impact_on)
		if (indic.impact.all_unset()) return;  // nothing to do
		else agenda.propagate(g,indic.impact);
	else
		agenda.init(g);

	int  c,v;                          // current constraint & variable
	int  old_c = -1;

	IntervalVector& box(_box);
	IntervalVector propbox(box);  // variables domains before last propagation

	/********************************/
	/* option "fine" propagation  */
	IntervalVector projbox(box); // variables domains before last projection
	/********************************/

	//   VECTOR thres(_nb_var);        // threshold for propagation
	//   for (int i=1; i<=_nb_var; i++) {
	//     thres(i) = ratio*Diam(box(i));
	//     if (thres(i)<w) thres(i)=w;
	//   }
	Indicators p(_box.size());
	p.scope_on=true;
	p.impact_on=true;

	while (!agenda.empty()) {

		agenda.pop(c,v);

		//cout << "Narrowing for (c" << c << ", v" << v << ")" << endl;
		//projbox(v+1) = box(v+1);

		if (c!=old_c)
			for (int i=0; i<g.ctr_nb_vars(c); i++) {
				int j=g.ctr_ith_var(c,i);
				projbox[j] = box[j];
			}

		p.scope.set(v);
		if (c==old_c) p.impact.unset_all();
		else {
			p.impact.set_all();
			old_c = c;
		}

		/********************************/
		/* "fine" propagation option  */
		/********************************/
		try {
			list[c].contract(box, p);
		}
		catch (EmptyBoxException& e) {
			agenda.flush();
			throw e;
		}

		//cout << "  =>" << box[v] << endl;
		//cout << agenda << endl;

		//if (propbox[v].rel_distance(box[v])>=ratio) {
		if (propbox[v].ratiodelta(box[v])>=ratio) {
			//cout << "before prop q=" << agenda << endl;
			/********************************/
			/* "fine" propagation option  */
			//if (projbox[v].rel_distance(box[v])>=ratio) {
			if (projbox[v].ratiodelta(box[v])>=ratio) {
				agenda.propagate(g,c,v);
			} else {
				/********************************/
				agenda.propagate(g,v); //(c,v);
				//       cout << "q=" << agenda << endl;
			}
			propbox[v] = box[v];
		}
	}

	/* we cancel the "residual" contractions
	 * in order to trigger more propagation next
	 * time this function is called. This action
	 * is meaningful when the contract floor is
	 * small w.r.t the ratio here. */
	//   if (!reducted) box = propbox; // restore domains

}

const REAL Propagation::default_ratio = __IBEX_DEFAULT_RATIO_PROPAG;

} // namespace ibex
