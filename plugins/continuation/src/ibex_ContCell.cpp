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
#include "ibex_Newton.h"

#include <cassert>
#include <iomanip>

using namespace std;

namespace ibex {

int ContCell::id_counter=0;
int ContCell::__total_facet_count=0;

ContCell::ContCell(const IntervalVector& existence_box, const IntervalVector& unicity_box, const IntervalVector& domain, const VarSet& vars) : unicity_box(unicity_box), existence_box(existence_box), vars(vars), h(vars.param_box(unicity_box).max_diam()), id(id_counter++) {
	create_facets(domain);
	__total_facet_count += facets.size();
}

void ContCell::diff(const IntervalVector& box, Function& f, const VarSet& box_vars) {
	ContCell& cell=*this;
	if (box.intersects(cell.unicity_box)) {

		bool same_vars=(((BitSet&) vars.is_var)==box_vars.is_var);

		for (list<Facet>::iterator it=cell.facets.begin(); it!=cell.facets.end(); ) {
			IntervalVector* result;
			//cout << "[diff] params:" << vars.param_box(*it) << " and " << vars.param_box(box) << endl;
			//cout << "       vars:" << vars.var_box(*it) << " and " << vars.var_box(box) << endl;

			if (!it->facet.overlaps(box)) { it++; continue; }

			// We force the diff to split first the parameters.
			// So we reorder the dimensions so that parameters appear first.

			IntervalVector reordered_facet=same_vars? vars.param_box(it->facet) : cart_prod(vars.param_box(it->facet), vars.var_box(it->facet));
			IntervalVector reordered_box=same_vars? vars.param_box(box) : cart_prod(vars.param_box(box), vars.var_box(box));

//			IntervalVector reordered_facet=cart_prod(vars.var_box(it->facet), vars.param_box(it->facet));
//			IntervalVector reordered_box=cart_prod(vars.var_box(box), vars.param_box(box));

			int nb_boxes=reordered_facet.diff(reordered_box,result);
			if (nb_boxes>0) {
				IntervalVector residu(it->facet); //box.size());
				for (int i=0; i<nb_boxes; i++) {

					if (same_vars)
						vars.set_param_box(residu,result[i]);
					else  {
						vars.set_param_box(residu,result[i].subvector(0,vars.nb_param-1));
						vars.set_var_box(residu,result[i].subvector(vars.nb_param,box.size()-1));

//						vars.set_var_box(residu,result[i].subvector(0,vars.nb_var-1));
//						vars.set_param_box(residu,result[i].subvector(vars.nb_var,box.size()-1));
//
//						if (nb_boxes>1 || (residu!=box)) {
//							// Contract the new boxes immediately
//							CtcParamNewton ctc(f,vars);
//							ctc.contract(residu);
//						}
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
			facets.push_back(Facet(i,false,facet));
		}
		p[i]=pi.ub();
		facet=vars.full_box(x,p) & domain;
		if (!facet.is_empty()) {
			facets.push_back(Facet(i,true,facet));
		}
	}
}

void ContCell::contract_facets(Ctc& ctc) {

	for (list<Facet>::iterator it=facets.begin(); it!=facets.end(); ) {
		ctc.contract(it->facet);
		if (it->facet.is_empty()) { // may happen because of the domain
			it=facets.erase(it);
			__total_facet_count--;
		} else
			it++;
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

void ContCell::find_solution_in_facets_not_in(Function& f, IntervalVector& px_sol, list<ContCell*> neighboors) {

	int n=f.nb_var();

	double min_p_width=0.0001*h; // TODO: remove hard-coded value 0.01?

	px_sol.set_empty(); // by default

	//cout << "[find-not-in] start. #neigbhoors=" << neighboors.size() << endl;

	for (list<Facet>::iterator itf=facets.begin(); itf!=facets.end(); ) {
		Facet& facet=*itf; // note: has already been Newton-contracted (see below)

		//cout << "[find-not-in] facet=" << facet.facet << endl;

		// ============ try to discard the whole subfacet ========================
		list<ContCell*>::iterator it=neighboors.begin();

		while (it!=neighboors.end() && !(*it)->unicity_box.is_superset(facet.facet)) {
			it++;
		}

		if (!neighboors.empty() && it!=neighboors.end()) { // already included in a cell
			//cout << "[find-not-in] facet discarded" << endl;
			itf=facets.erase(itf);
			__total_facet_count--;
			continue;
		}
		// ===============================================================================


		// ============ Build a sample solution inside the subfacet ========================

		IntervalVector x(vars.var_box(facet.facet));
		IntervalVector p(vars.param_box(facet.facet));
		bool solution_found;

		// -------------------------------------------------------------------------------
		// with inflating Newton
		// -------------------------------------------------------------------------------
		IntervalVector __ignore__(n);
		IntervalVector existence(n);
		solution_found = inflating_newton(f,vars,vars.full_box(x,p.mid()),existence,__ignore__);

		if (!solution_found) {
			cout << "[find-not-in] Fatal: inflating Newton failed\n";
			exit(-1);
		}

		if (!vars.var_box(existence).is_subset(vars.var_box(facet.facet))) {
			solution_found=false;
			// may happen because
			// the facets are intersected with the domain. Some
			// values for parameters may have no more associated
			// solutions for the variables.
		}
		// -------------------------------------------------------------------------------


		// -------------------------------------------------------------------------------
		// with contracting Newton
		// -------------------------------------------------------------------------------
//		IntervalVector existence=vars.full_box(x,p.mid());
//
//		newton(f,vars,existence,NEWTON_CTC_PREC);
//
//		// Warning: Newton can give an empty box here because
//		// the facets are intersected with the domain. Some
//		// values for parameters may have no more associated
//		// solutions for the variables.
//
//
//		solution_found = !existence.is_empty() && vars.var_box(existence).is_interior_subset(x);
//
//		if (!solution_found) {
////			cout << "[find-not-in] Newton gives no strict contraction...\n";
//		}
		// -------------------------------------------------------------------------------

		// ============ check that this solution belongs to no cell ========================
		if (solution_found) {
			it=neighboors.begin();

			while (it!=neighboors.end() && !(*it)->unicity_box.intersects(existence)) {
				it++;
			}

			if (neighboors.empty() || it==neighboors.end()) { // belongs to no cell ---> OK!
				//cout << "[find-not-in] solution found!" << endl;
				px_sol=existence;
				return;
			}
		}
		// ===============================================================================

		// ============================ bisect the subfacet ==============================
		if (p.max_diam()<min_p_width) {
			facets.push_front(Facet(facet.p,facet.sign,facet.facet)); // by copy
			itf=facets.erase(itf);
			continue;
		}

		pair<IntervalVector,IntervalVector> p12=p.bisect(p.extr_diam_index(false));

		IntervalVector px1(vars.full_box(x,p12.first));
		IntervalVector px2(vars.full_box(x,p12.second));
		// Contract the new boxes immediately
		newton(f,vars,px1,NEWTON_CTC_PREC);
		newton(f,vars,px2,NEWTON_CTC_PREC);

		if (!px1.is_empty()) { // may happen because of the domain
			facets.push_back(Facet(facet.p,facet.sign,px1));
			__total_facet_count++;
		}
		if (!px2.is_empty()) { // may happen because of the domain
			facets.push_back(Facet(facet.p,facet.sign,px2));
			__total_facet_count++;
		}
		// **note** the current facet must be erased *after*
		// the sub-facets are pushed (for the case where itf
		// points to the last element of the list).
		itf=facets.erase(itf);
		__total_facet_count--;
		// ===============================================================================
	}

	if (!facets.empty()) {

		// we first try to return a degenerated box that does not belong
		// to any neighbor. Contrary to before, it is not proven anymore
		// that this box contain a solution (and it probably doesn't since
		// it is degenerated), which may result in a "choose fail".
		for (list<Facet>::iterator itf=facets.begin(); itf!=facets.end(); itf++) {
			list<ContCell*>::iterator it=neighboors.begin();
			while (it!=neighboors.end() && !(*it)->unicity_box.intersects(itf->facet)) {
				it++;
			}
			if (neighboors.empty() || it==neighboors.end()) {
				px_sol=itf->facet;
				return;
			}
		}

		// if this does not succeed:
		throw FindSolutionFail();
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
