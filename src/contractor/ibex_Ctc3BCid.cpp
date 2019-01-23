//============================================================================
//                                  I B E X                                   
// File        : ibex_Ctc3BCID.cpp
// Author      : Ignacio Araya, Gilles Chabert,
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 20, 2018
//============================================================================

#include "ibex_Ctc3BCid.h"

using namespace std;
namespace ibex {

Ctc3BCid::Ctc3BCid(const BitSet& cid_vars, Ctc& ctc, int s3b, int scid, int vhandled, double var_min_width) :
									Ctc(ctc.nb_var), cid_vars(cid_vars), ctc(ctc), s3b(s3b), scid(scid),
									vhandled(vhandled<=0? cid_vars.size():vhandled),
									var_min_width(var_min_width), start_var(0) {
	assert(ctc.nb_var>0);
	//	if (ctc.nb_var<=0)
	//		ibex_error("Ctc3BCID : the contractor is non-dimensional, Please specify the dimension with: \n Ctc3BCid(int nb_var, const BoolMask& cid_vars, Ctc& ctc, int s3b, int scid, int vhandled, double var_min_width);");
}


Ctc3BCid::Ctc3BCid(Ctc& ctc, int s3b, int scid, int vhandled, double var_min_width) :
                    				Ctc(ctc.nb_var), cid_vars(BitSet::all(nb_var)), ctc(ctc), s3b(s3b), scid(scid),
									vhandled(vhandled<=0? nb_var : vhandled),
									var_min_width(var_min_width), start_var(0), context(NULL) {

	assert(ctc.nb_var>0);
	//	if (ctc.nb_var<=0)
	//		ibex_error("Ctc3BCID : the contractor is non-dimensional, Please specify the dimension with: \n Ctc3BCid(int nb_var, Ctc& ctc, int s3b, int scid, int vhandled, double var_min_width);");
}

Ctc3BCid::~Ctc3BCid() {

}

void Ctc3BCid::add_property(const IntervalVector& init_box, BoxProperties& map) {
	ctc.add_property(init_box, map);
}

int Ctc3BCid::limitCIDDichotomy ()  {
	return LimitCIDDichotomy;
}

/* compare the boxes in all dimensions except one (var) */
bool Ctc3BCid::equalBoxes (int var, IntervalVector &box1, IntervalVector &box2) {
	int nb_var = box1.size();

	for(int j=0; j<nb_var; j++) {
		if(j!=var && box1[j]!=box2[j]) return false;
	}

	return true;
}

void Ctc3BCid::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void Ctc3BCid::contract(IntervalVector& box, ContractContext& context) {
	int var;                                           // [gch] variable to be carCIDed

	start_var=nb_var-1;
	//  patch pour l'optim  A RETIRER ??

	this->context = &context;

	for (int k=0; k<vhandled; k++) {                   // [gch] k counts the number of varCIDed variables [gch]

		var=(start_var+k)%nb_var;

		var3BCID(box, var);

		if (box.is_empty()) {
			context.output_flags.add(FIXPOINT);
			this->context = NULL;
			return;
		}
	}

	context.prop.update(BoxEvent(box,BoxEvent::CONTRACT));
	this->context = NULL;
	//	start_var=(start_var+vhandled)%nb_var;             //  en contradiction avec le patch pour l'optim
}


bool Ctc3BCid::var3BCID(IntervalVector& box, int var) {

	Interval& dom(box[var]);

	if (dom.diam() < var_min_width) return false;      // domain already small enough : nothing to do
	if (dom.diam() == POS_INFINITY) return false;     //  no handling infinite domains
	double w_DC = dom.diam() / s3b;
	int locs3b=s3b;

	// computes the actual number of slices s3b and the actual width of a slice w_DC,
	// taking into account the minimum width of the slice and the
	// maximum number of slices
	if (w_DC < var_min_width) {
		locs3b = (int) (dom.diam()/var_min_width);
		if (locs3b < 2)  locs3b=2;
		w_DC= dom.diam()/ locs3b;
	}

	// depending of the actual number of slices, calls the 3BCID contractor with a dichotomic or a linear shaving.
	if (locs3b > limitCIDDichotomy())
		return var3BCID_dicho(box, var, w_DC);
	else
		return var3BCID_slices(box, var, locs3b, w_DC, dom);
}

bool Ctc3BCid::var3BCID_dicho(IntervalVector& box, int var, double w3b) {
	IntervalVector initbox = box;

	// left shaving, after box contains the left slice
	bool r0= shave_bound_dicho(box, var, w3b, true);

	if (box.is_empty()) return true;
	if (box[var].ub() == initbox[var].ub())
		return true;                                   // the left slice reaches the right bound : nothing more to do

	IntervalVector leftbox=box;
	box=initbox;
	box[var]= Interval(leftbox[var].lb(),initbox[var].ub());
	bool r1=false;

	r1= shave_bound_dicho (box, var,  w3b, false); // may result in an empty box

	if (box.is_empty()) {                 // in case of empty box in the right shaving,
		box=leftbox;                      // the contracted box becomes the left box
		return true;
	}

	IntervalVector rightbox=box;
	box=initbox;
	box[var]= Interval(leftbox[var].ub(),rightbox[var].lb()); // the central part
	IntervalVector savebox=box;
	IntervalVector newbox= leftbox | rightbox;         // the hull

	if (varCID(var,savebox,newbox)) {
		box = newbox; return true;                     // the contracted box is in newbox
	}
	else {                                             // VarCID was useless : one returns the result of only 3B:
		box = initbox;                                 // var is the only contracted variable
		box[var] = Interval(leftbox[var].lb(),rightbox[var].ub());
		return (r0 | r1);
	}
}

void Ctc3BCid::update_and_contract(IntervalVector& box, int var) {

	ContractContext sub_context(box, *context);

	// ----------------------------------------------------------------
	// Note: the initial impact (that of Ctc3BCid) should be maintained
	// (the next line should be removed) if there is no initial
	// contraction before the slicing process (see comment in
	// CtcAcid::contract). However, it is more efficient in practice
	// to only set the split variable in the impact, with or without
	// initial contraction.
	sub_context.impact.clear();
	// ----------------------------------------------------------------

	sub_context.impact.add(var);

	// By creating a "slice", we have contracted only the domain of
	// a single variable:
	BoxEvent event(box, BoxEvent::CONTRACT, BitSet::singleton(nb_var,var));

	sub_context.prop.update(event);

	ctc.contract(box,sub_context);
}

bool Ctc3BCid::shave_bound_dicho(IntervalVector& box, int var, double wv, bool left) {

	IntervalVector initbox = box;
	Interval& x(box[var]);


	double inf = x.lb();                                // inf bound (to increase)
	double sup = x.ub();                                // sup bound (to decrease)
	double inf0 = inf;
	double sup0 = sup;
	volatile double tmp;

	//cout.precision(20);
	//	cout << endl << "Shaving variable " << var << " bound " << (left?"left":"right") <<  " inf "  << inf <<  " sup  " << sup << endl;

	if (left) {

		double lb  = x.mid();                          // left bound of the border
		double rb  = sup;                              // right bound of the border (should decrease even when shaving
		// left, thanks to the "bound" test -not yet-)
		while (1) {

			//		        cout << " left  inf=" << inf << " lb=" << lb << " rb=" << rb << " sup=" << sup << endl;
			box[var] = Interval(inf,lb);

			update_and_contract(box, var);

			if (!box.is_empty()) {
				inf=box[var].lb();
				volatile double mid = (inf+lb)/2;      // we must subdivide the current slice (declared volatile to prevent
				//   the compiler from expanding mid in the next line and using higher
				//   precision than double in the comparisons (messing up the result))
				if (lb-inf<=wv || inf>=mid || lb<=mid) // the two last conditions prevent from splitting a float in half
					break;
				else lb=mid;                           // useless to restore domains (we divide the same slice)
			} else {                                   // the current slice has been cut off
				//	cout << "      slice removed.\n";
				if (inf==lb) {                         // border is degenerated and current=border
					if (inf==sup) {                    // current=border=the whole interval itself:
						box.set_empty() ; return true; //   in this case the box must remain entirely emptied
					}
					else break;                        // return anyway (no more to do).
				}
				tmp = inf;                             // current value of inf is used two lines below, save it
				inf = lb;                              // increase the inf bound
				// lb = 3*lb-2*tmp;                    // optimistic choice: we double the width of the slice
				lb = 2*lb-tmp;                         // more realistic  choice: we take the width of the slice
				if (lb>rb) lb = rb;                    // the largest possible: lb<-rb => try the whole border interval once
				box = initbox;                         // restore domains (slice has changed)
			}

		}
	} else {

		double rb  = x.mid();                          // right bound of the border
		double lb  = inf;                              // left bound of the border (should increase even when shaving
		// right, thanks to the "bound" test -not yet-)
		while (1) {
			//			     cout << " right  inf=" << inf << " lb=" << lb << " rb=" << rb << " sup=" << sup << endl;
			box[var] = Interval(rb,sup);

			update_and_contract(box, var);

			if (!box.is_empty()) {
				sup=box[var].ub();
				volatile double mid = (rb+sup)/2;      // we must subdivide the current interval (declared volatile to prevent
				//   the compiler from expanding mid in the next line and using higher
				//   precision than double in the comparisons (messing up the result))
				if (sup-rb<=wv || rb>=mid || sup<=mid) // the two last conditions prevent from splitting a float in half
					break;
				else rb=mid;                           // useless to restore domains (we divide the same slice)
			} else {                                   // the current slice has been cut off
				//cout << "      slice removed.\n";
				if (sup==rb) {                         // border is degenerated and current=border
					if (inf==sup)                      // current=border=the whole interval itself:
						return true;                   //   in this case the box must remain entirely emptied
					else break;                        // return anyway (no more to do).
				}
				tmp = sup;                             // current value of sup is used two lines below, save it
				sup = rb;                              // decrease the sup bound
				//   	rb = 3*rb-2*tmp;               // optimistic choice: we double the width of the slice
				rb = 2*rb-tmp;                         // more realistic  choice: we take the width of the slice
				if (rb<lb)  rb = lb;                   // the largest possible: rb<-lb => try the whole border interval once
				box = initbox;                         // restore domains (slice has changed)
			}
		}
	}
	if (inf < inf0 +wv && sup > sup0 -wv) return false;
	else return true;
}

bool Ctc3BCid::var3BCID_slices(IntervalVector& box, int var, int locs3b, double w_DC, Interval& dom) {

	IntervalVector savebox(box);

	// Reduce left bound by shaving:

	bool stopLeft = false;
	double leftBound = dom.lb();
	double rightBound = dom.ub();
	double leftCID=0.;

	int k=0;

	while (k < locs3b && ! stopLeft) {

		// Compute a slice 'dom'
		if (k > 0) box = savebox;
		double inf_k = dom.lb()+k*w_DC;
		double sup_k = dom.lb()+(k+1)*w_DC;
		if (sup_k > dom.ub() || (k == locs3b - 1 && sup_k<dom.ub())) sup_k = dom.ub();
		dom = Interval(inf_k, sup_k);

		// Try to refute this slice

		update_and_contract(box, var);

		if (box.is_empty()) {
			leftBound = sup_k;
			k++;
			continue;
		}
		//non empty box
		stopLeft = true;
		leftCID = sup_k;
		leftBound = dom.lb();
		k++;
	}

	if (!stopLeft) {                                   // all slices give an empty box
		box.set_empty();
		return true;                                   // TODO: check return value
	} else if (k == locs3b) {
		// Only the last slice gives a non-empty box : box is reduced to this last slice
		return true;
	} else {

		IntervalVector newbox (box);                   // newbox is initialized with the last slice handled in the previous loop
		int kLeft = k-1;

		// Reduce right bound by shaving:
		bool stopRight = false;
		double lastInf_k=0.;

		int k2 = locs3b - 1;

		while (k2 > kLeft && ! stopRight) {

			// Compute slice
			box = savebox;
			double inf_k = dom.lb() + k2 * w_DC;
			double sup_k = dom.lb() + (k2+1) * w_DC;
			if (sup_k > dom.ub() || (k2 == locs3b - 1 && sup_k < dom.ub())) sup_k = dom.ub();

			dom = Interval(inf_k, sup_k);

			// Try to refute the slice

			update_and_contract(box, var);

			if (box.is_empty()) {
				rightBound = sup_k;
				k2--;
				continue;
			}

			stopRight = true;
			lastInf_k = inf_k;
			rightBound = dom.ub();

			k2--;

		}

		if (!stopRight) {                              // All the boxes visited in the second loop give an empty box
			box = newbox;
			return true;
		} else {

			int kRight = k2 +1;

			if (kLeft + 1 == kRight) {                 // No slice between the last handled left and right slices
				box = box | newbox;
				return true;
			}

			else {                                     // A varCID operation is performed on the remaining box :

				newbox = newbox | box;
				savebox[var]=Interval(leftCID, lastInf_k);

				if(varCID(var,savebox,newbox)) {       // Call to the central CID contraction
					box = newbox;
				} else {                               // VarCID was useless : one returns the result of only 3B
					box = savebox;
					box[var] = Interval(leftBound, rightBound);
				}
			}
		}
	}

	return true;
}


bool Ctc3BCid::varCID(int var, IntervalVector &varcid_box, IntervalVector &var3Bcid_box) {

	if(scid==0 || equalBoxes (var, varcid_box, var3Bcid_box)) return false;

	IntervalVector box=varcid_box;
	Interval& dom(box[var]);

	double w_DC = dom.diam() / scid;
	for (int k = 0 ; k < scid ; k++) {
		if(k>0) box=varcid_box;
		// compute slice:
		double inf_k = dom.lb() + k * w_DC;
		double sup_k = dom.lb() + (k+1) * w_DC;
		if (sup_k > dom.ub() || (k == scid-1 && sup_k < dom.ub())) sup_k = dom.ub();
		dom = Interval(inf_k, sup_k);

		update_and_contract(box, var);

		if (box.is_empty()) {
			continue;                                  // the current slice is infeasible : nothing to add to the hull
		}

		var3Bcid_box |= box;                           // add box to the hull
		if(equalBoxes (var, varcid_box, var3Bcid_box))
			return false;                              // VarCID was useless
	}

	return true;
}


} // end namespace ibex
