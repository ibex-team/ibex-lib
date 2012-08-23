//============================================================================
//                                  I B E X                                   
// File        : ibex_Optimizer.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_Optimizer.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Timer.h"
#include "ibex_OptimProbing.cpp_"
#include "ibex_OptimSimplex.cpp_"
#include "ibex_CtcProj.h"
#include "ibex_ExprCopy.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

const double Optimizer::default_prec = 1e-07;
const double Optimizer::default_goal_rel_prec = 1e-07;
const double Optimizer::default_goal_abs_prec = 1e-07;
const int Optimizer::default_sample_size = 10;
const char* Optimizer::goal_name = "y";

void Optimizer::write_ext_box(const IntervalVector& box, IntervalVector& ext_box) {
	int i2=0;
	for (int i=0; i<n; i++,i2++) {
		if (i2==goal_var) i2++; // skip goal variable
		ext_box[i2]=box[i];
	}
}

void Optimizer::read_ext_box(const IntervalVector& ext_box, IntervalVector& box) {
	int i2=0;
	for (int i=0; i<n; i++,i2++) {
		if (i2==goal_var) i2++; // skip goal variable
		box[i]=ext_box[i2];
	}
}

Optimizer::Optimizer(System& user_sys, Bsc& bsc, Ctc& ctc, double prec,
		double goal_rel_prec, double goal_abs_prec, int sample_size) :
		n(user_sys.f.input_size()), m(user_sys.ctrs.size()),
		sys(user_sys,System::NORMALIZE),
		goal_ctr(0), goal_var(n), bsc(bsc), ctc(ctc), buffer(n),
		prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
		sample_size(sample_size), mono_analysis_flag(true), in_HC4_flag(true), trace(false),
		timeout(1e08), loup(POS_INFINITY), loup_point(n),
		uplo_of_epsboxes(POS_INFINITY) {

	// ====== build the reversed inequalities g_i(x)>0 ===============
	Array<Ctc> ng(m);
	for (int i=0; i<m; i++) {
		ng.set_ref(i, *new CtcProj(sys.f[i],NumConstraint::GT));
	}
	is_inside=new CtcUnion(ng);
	// =============================================================

	if (trace) cout.precision(12);

	nb_simplex=0;
	diam_simplex=0;
	nb_rand=0;
	diam_rand=0;
}

Optimizer::~Optimizer() {

	for (int i=0; i<m; i++) {
		delete &(is_inside->list[i]);
	}

	delete is_inside;
}

bool Optimizer::update_loup(const IntervalVector& box) {
	int box_loup_changed = update_loup_probing (box);
	box_loup_changed |= update_loup_simplex(box);
	return box_loup_changed;
}

bool Optimizer::contract_and_bound(Cell& c) {

	/*======================== contract y with y<=loup ========================*/
	Interval& y=c.box[goal_var];

	//cout << "loup=" << loup << endl;

	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax=loup - goal_rel_prec*fabs(loup);

	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;

	//cout << "ymax=" << ymax << endl;

	if (y.lb() > ymax) {
		c.box.set_empty();
		throw EmptyBoxException();
	}
	if (y.ub() > ymax) y = Interval(y.lb(),ymax);

	/*================ contract x with f(x)=y and g(x)<=0 ================*/
//	cout << "y=f(x) and g(x)<=0 " << endl;
//	cout << "   x before=" << c.box << endl;
//	cout << "   y before=" << y << endl;

	ctc.contract(c.box); // may throw EmptyBoxException

//	cout << "   x after=" << c.box << endl;
//	cout << "   y after=" << y << endl;
	// TODO: no more cell in argument here (just a box). Does it matter?
	/*====================================================================*/

	// there is still something left to be contracted in the box
	/*========================= update loup ==============================*/
	IntervalVector tmp_box(n);
	read_ext_box(c.box,tmp_box);
	bool loup_changed = update_loup(tmp_box);
	write_ext_box(tmp_box,c.box);
	/*====================================================================*/

	if (tmp_box.max_diam()<=prec) {
		// rem1: tmp_box and not c.box because y is [-inf;loup]
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)

		// the box is a "solution"
		if (uplo_of_epsboxes > y.lb()) {
			if (trace) {
				cout.precision(12);
				cout << "uplo of eps-boxes:" << y.lb() << endl;
			}
			uplo_of_epsboxes = y.lb();
		}
		throw EmptyBoxException();
	}

	return loup_changed;
}

void Optimizer::optimize(const IntervalVector& init_box) {

	buffer.flush();

	Cell* root=new Cell(IntervalVector(n+1));

	write_ext_box(init_box,root->box);

	// add data required by the contractor
	//ctc.init_root(*root); // we know there is none (not incremental HC4).

	// add data required by the bisector
	bsc.init_root(*root);

	try {
		contract_and_bound(*root);
	} catch(EmptyBoxException&) {
		delete root;
		return;
	}

	buffer.push(root);

	while (!buffer.empty()) {

		//if (trace) cout << ((CellBuffer&) buffer) << endl;

		Cell* c=buffer.top();

		pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
		pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

		delete buffer.pop();

		bool loup_changed;

		try {
			loup_changed = contract_and_bound(*new_cells.first);
			buffer.push(new_cells.first);
		} catch(EmptyBoxException&) {
			delete new_cells.first;
		}

		try {
			loup_changed |= contract_and_bound(*new_cells.second);
			buffer.push(new_cells.second);
		} catch(EmptyBoxException&) {
			delete new_cells.second;
		}

		if (loup_changed) {
			// In case of a new upper bound (loup_changed == true), all the boxes
			// with a lower bound greater than (loup - goal_prec) are removed and deleted.
			double ymax=loup - goal_rel_prec*fabs(loup);
			if (loup - goal_abs_prec < ymax)
				ymax = loup - goal_abs_prec;
			((CellHeap&) buffer ).contract_heap(ymax);

			if (trace) cout << "ymax=" << ymax << " uplo=" << ((CellHeap&) buffer ).minimum() << endl;
		}
		// Note: if contraction was before bisection, we could have the problem
		// that the current cell is removed by contract_heap. See comments in
		// older version of the code (before revision 284).

		Timer::check(timeout);
	}
}

void Optimizer::report() {
	double uplo=loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < uplo)
		uplo = loup - goal_abs_prec;

	if (uplo > uplo_of_epsboxes) uplo=uplo_of_epsboxes;

	cout << " best bound in: [" << uplo << "," << loup << "]" << endl;

	double rel_prec;
	if (loup==POS_INFINITY) rel_prec=0;
	else rel_prec=(loup-uplo)/(fabs (loup));

	double abs_prec=loup-uplo;

	cout << " relative precision obtained on objective function: " << rel_prec << " " <<
			(rel_prec < goal_rel_prec? " [passed]" : " [failed]") << endl;

	cout << " absolute precision obtained on objective function: " << abs_prec << " " <<
			(abs_prec < goal_abs_prec? " [passed]" : " [failed]") << endl;
}

} // end namespace ibex
