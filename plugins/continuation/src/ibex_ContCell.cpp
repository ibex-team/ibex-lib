/* ============================================================================
 * I B E X - Continuation cells
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#include "ibex_ContCell.h"
#include "ibex_ParametricProof.h"
#include "ibex_Cont.h"

#include <cassert>

using namespace std;

namespace ibex {

int ContCell::id_counter=0;
int ContCell::__total_facet_count=0;

ContCell::ContCell(const IntervalVector& existence_box, const IntervalVector& unicity_box, const IntervalVector& domain, const VarSet& vars) : unicity_box(unicity_box), existence_box(existence_box), vars(vars), h(vars.param_box(unicity_box).max_diam()), id(id_counter++) {
	create_facets(domain);
	__total_facet_count += facets.size();
}

void ContCell::diff(const IntervalVector& box, Function& f) {
	ContCell& cell=*this;
	if (box.intersects(cell.unicity_box)) {
		for (list<Facet>::iterator it=cell.facets.begin(); it!=cell.facets.end(); ) {
			IntervalVector* result;
			//cout << "[diff] params:" << vars.param_box(*it) << " and " << vars.param_box(box) << endl;
			//cout << "       vars:" << vars.var_box(*it) << " and " << vars.var_box(box) << endl;

			// We force the diff to split first the parameters.
			// So we reorder the dimensions so that parameters appear first.
			IntervalVector reordered_facet=cart_prod(vars.param_box(it->facet), vars.var_box(it->facet));
			IntervalVector reordered_box=cart_prod(vars.param_box(box), vars.var_box(box));

//			IntervalVector reordered_facet=cart_prod(vars.var_box(it->facet), vars.param_box(it->facet));
//			IntervalVector reordered_box=cart_prod(vars.var_box(box), vars.param_box(box));

			int nb_boxes=reordered_facet.diff(reordered_box,result);
			if (nb_boxes>0) {
				for (int i=0; i<nb_boxes; i++) {
					IntervalVector residu(box.size());
					vars.set_param_box(residu,result[i].subvector(0,vars.nb_param-1));
					vars.set_var_box(residu,result[i].subvector(vars.nb_param,box.size()-1));

//					vars.set_var_box(residu,result[i].subvector(0,vars.nb_var-1));
//					vars.set_param_box(residu,result[i].subvector(vars.nb_var,box.size()-1));

					if (nb_boxes>1 || (residu!=box)) {
						// Contract the new boxes immediately
						CtcParamNewton ctc(f,vars);
						ctc.contract(residu);
					}
					if (!residu.is_empty()) {
						cell.facets.push_front(Facet(it->p, it->sign, residu));
						__total_facet_count++;
					}
					//cout << "         " << residu << endl;
				}
			}
			//cout << "================================================================\n";
			delete[] result;
			it=cell.facets.erase(it);
			__total_facet_count--;
		}
	}
}

//void ContCell::add_facet(const IntervalVector& box) {
//	ContCell& cell=*this;
//	list<IntervalVector> res;
//	res.push_front(box);
//	for (list<IntervalVector>::iterator it=cell.facets.begin(); it!=cell.facets.end(); it++) {
//		for (list<IntervalVector>::iterator it2=res.begin(); it2!=res.end(); ) {
//			IntervalVector* result;
//			int nb_boxes=it2->diff(*it,result);
//			if (nb_boxes>0) {
//				for (int i=0; i<nb_boxes; i++) {
//					res.push_front(result[i]);
//				}
//			}
//			delete[] result;
//			it2=res.erase(it2);
//		}
//	}
//
//	for (list<IntervalVector>::iterator it2=res.begin(); it2!=res.end(); it2++) {
//		facets.push_back(*it2);
//	}
//
//}

ContCell::Facet::Facet(int p, bool sign, const IntervalVector& facet) : p(p), sign(sign), facet(facet) {

}

void ContCell::create_facets(const IntervalVector& domain) {

	assert(facets.empty());
	IntervalVector x=vars.var_box(existence_box);
	IntervalVector facet(x);
	int m=vars.nb_param;
	for (int i=0; i<m; i++) {
		IntervalVector p=vars.param_box(existence_box);
		Interval pi=p[i];
		p[i]=pi.lb();
		facet=vars.full_box(x,p) & domain;
		if (!facet.is_empty()) {
			facets.push_back(Facet(i,false,vars.full_box(x,p)));
		}
		p[i]=pi.ub();
		facet=vars.full_box(x,p) & domain;
		if (!facet.is_empty()) {
			facets.push_back(Facet(i,true,vars.full_box(x,p)));
		}
	}
}

void ContCell::contract_facets(Ctc& ctc) {
	for (list<Facet>::iterator it=facets.begin(); it!=facets.end(); it++) {
		ctc.contract(it->facet);
	}
}

void ContCell::find_solution_in_facets(Function& f, IntervalVector& x) {
	x.set_empty();

	while (!facets.empty()) {
		//cout << "[find_solution] try next facet" << endl;
		x = find_solution(f, facets.front().facet, vars);
		// No solution in the facet => remove the facet
		if (x.is_empty()) {
			facets.pop_front();
			__total_facet_count--;
		} else {
			return;
		}
	}
}

void ContCell::check_no_facet_contains(const IntervalVector& x) {
	for (list<Facet>::const_iterator it=facets.begin(); it!=facets.end(); it++) {
		if (it->facet.intersects(x)) {
			cerr << "Error: a facet still contains x=" << x << endl;
			cerr << "facet=" << it->facet << endl;
			exit(1);
		}
	}
}

ostream& operator<<(ostream& os, const ContCell& cell) {
	os << "\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500	\n";
	os << "\u2502Unicity box=" << cell.unicity_box << endl;
	os << "\u2502" << cell.facets.size() << " facets:" << endl;
	for (list<ContCell::Facet>::const_iterator it=cell.facets.begin(); it!=cell.facets.end(); it++) {
		os << "\u2502  " << it->facet << endl;
	}
	os << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500	\n";
	return os;
}

} /* namespace ibex */
