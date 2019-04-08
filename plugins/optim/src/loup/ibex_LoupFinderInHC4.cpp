//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderInHC4.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinderInHC4.h"
#include "ibex_LoupFinderProbing.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_BxpSystemCache.h"

using namespace std;

namespace ibex {

LoupFinderInHC4::LoupFinderInHC4(const System& sys) : sys(sys), goal_ctr(-1) {
	mono_analysis_flag=true;
//	nb_inhc4=0;
//	diam_inhc4=0;
//	nb_rand=0;
//	diam_rand=0;

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
	}

}

//void LoupFinderInHC4::report() {
//    if (trace) {
//      cout << " nbrand " << nb_rand << " nb_inhc4 " << nb_inhc4 << " nb simplex " << nb_simplex << endl;
//      cout << " diam_rand " << diam_rand << " diam_inhc4 " << diam_inhc4 << " diam_simplex " << diam_simplex << endl;
//    }
//}

void LoupFinderInHC4::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	//--------------------------------------------------------------------------
	/* Using system cache seems not interesting. */
//	if (!prop[BxpSystemCache::get_id(sys)]) {
//		prop.add(new BxpSystemCache(sys,BxpSystemCache::default_update_ratio));
//	}
	//--------------------------------------------------------------------------
}

std::pair<IntervalVector, double> LoupFinderInHC4::find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {

	IntervalVector inbox=box;
	bool inner_found=true;

	if (sys.nb_ctr>0) {

		// ========= get active constraints ===========
		BitSet* active;

		//--------------------------------------------------------------------------
		/* Using system cache seems not interesting. */
		//BxpSystemCache* p=(BxpSystemCache*) prop[BxpSystemCache::get_id(sys)];
		BxpSystemCache* p=NULL;
		//--------------------------------------------------------------------------

		if (p!=NULL) {
			active = &p->active_ctrs();
//			if (active->size()<box.size()) {
//				cout << "[inhc4] inactive constraint!\n";
//			}
		} else {
			active = new BitSet(sys.active_ctrs(box));
		}
		// ============================================

		if (!active->empty()) {

			//IntervalVector gx = sys.active_ctrs_eval(box);
			IntervalVector gx = p? p->active_ctrs_eval() : sys.f_ctrs.eval_vector(box,*active);

			int c;
			for (int i=0; i<active->size(); i++) {
				c=(i==0? active->min() : active->next(c));

				Interval right_cst;
				switch(sys.ops[c]) {
				case LT :
				case LEQ : right_cst=Interval::NEG_REALS; break;
				case EQ  :
					if (c==goal_ctr) // ---> f(x)<=y
						right_cst=Interval::NEG_REALS;
					else
						right_cst=Interval::ZERO;      break;
				case GEQ :
				case GT : right_cst=Interval::POS_REALS;  break;
				}

				// Quick infeasibility check
				if (gx[i].is_disjoint(right_cst)) throw NotFound();

				// *******
				// Warning: generates components of f_ctrs!!
				// *******
				sys.f_ctrs[c].ibwd(right_cst, inbox);

				if (inbox.is_empty()) {
					inner_found=false;
					break;
				}
			}
		}
		if (!p)
			delete active;
	}

//	if (inner_found) {
//		cout <<  " inner box found:" << inbox << endl;
//		nb_inhc4++;
//		diam_inhc4 = ((nb_inhc4-1) *diam_inhc4 + inbox.max_diam()) / nb_inhc4;
//	} else {
//		nb_rand++;
//		diam_rand = ((nb_rand-1) *diam_rand + box.max_diam()) / nb_rand;
//	}

	// if we reach this point: "inbox" is an inner box
	if (mono_analysis_flag)
		monotonicity_analysis(sys, inbox, inner_found);

	return LoupFinderProbing(sys).find(inner_found? inbox : box, loup_point, loup);
}

} /* namespace ibex */
