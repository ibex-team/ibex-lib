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

namespace ibex {

const double Optimizer::default_prec = 1e-07;
const double Optimizer::default_goal_rel_prec = 1e-02;
const double Optimizer::default_goal_abs_prec = 1e-07;
const int Optimizer::default_sample_size = 10;

Optimizer::Optimizer(Function& f, const Array<Function>& g, Ctc& ctc, Bsc& bsc, double prec,
		double goal_rel_prec, double goal_abs_prec, int sample_size) :
		n(f.nb_symbols()), f(f), g(g), ctc(ctc), bsc(bsc), buffer(),
		prec(n,prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
		sample_size(sample_size), mono_analysis_flag(true), in_HC4_flag(true), trace(false),
		f_eval(f), f_grad(f), loup(POS_INFINITY), loup_point(n), uplo_of_epsboxes(POS_INFINITY) {

	//g_eval...
	// is_inside...
	if (trace) cout.precision(12);
}

bool Optimizer::update_loup(const IntervalVector& box) {
	int box_loup_changed = update_loup_probing (box);
	box_loup_changed |= update_loup_simplex(box);
	return box_loup_changed;
}

bool Optimizer::contract_and_bound(Cell& c) {

	/*======================== contract with loup ========================*/
	Interval& y=c.get<OptimCrit>().y;
	double ymax=loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;

	if (y.lb() > ymax) {
		c.box.set_empty();
		throw EmptyBoxException();
	}
	if (y.ub() > ymax) y = Interval(y.lb(),ymax);
	/*====================================================================*/


	/*======================== contract with g(x)<=0 =====================*/
	ctc.contract(c); // may throw EmptyBoxException
	/*====================================================================*/

	// there is still something left to be contracted in the box
	/*========================= update loup ==============================*/

	bool loup_changed = update_loup(c.box);
	/*====================================================================*/

	try {
		prec.contract(c.box);
	} catch (EmptyBoxException&) { // the box is a "solution"
		y=f_eval.eval_scalar(c.box);
		if (uplo_of_epsboxes > y.lb()) uplo_of_epsboxes = y.lb();
		throw EmptyBoxException();
	}

	return loup_changed;
}

void Optimizer::optimize(const IntervalVector& init_box) {

	buffer.flush();

	Cell* root=new Cell(init_box);

	// add data required by the buffer
	root->add<OptimCrit>();

	// add data required by the contractor
	ctc.init_root(*root);

	// add data required by the bisector
	bsc.init_root(*root);

	buffer.push(root);

	while (!buffer.empty()) {
		Cell* c=buffer.top();

		bool loup_changed=contract_and_bound(*c);

		if (loup_changed) {
			// In case of a new upper bound (loupchange = true), all the boxes
			// with a lower bound greater than (loup - goal_prec) are removed and deleted.
			double ymax=loup - goal_rel_prec*fabs(loup);
			if (loup - goal_abs_prec < ymax)
				ymax = loup - goal_abs_prec;
			((CellHeap&) buffer ).contract_heap(ymax);

			if (trace) cout << "uplo " << ymax << endl;
		}
		// Note: in principle, even if the loup has changed, the current
		// cell "c" should not been removed by the previous call to contract_heap.
		// because this cell is precisely the one where a new loup has been found.
		// However, because we of goal_xxx_ceil, it can be removed. But, in the
		// latter case, the buffer is necessarily empty: this cell is the one with
		// the lowest lower bound on the criterion. If it has been removed, all
		// the subsequent cells in the heap are removed.
		if (!buffer.empty()) {
			pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
			pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

			delete buffer.pop();
			buffer.push(new_cells.first);
			buffer.push(new_cells.second);
		}

		Timer::check(timeout);
	}
}

void Optimizer::report() {
	double uplo=loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < uplo)
		uplo = loup - goal_abs_prec;

	if (uplo > uplo_of_epsboxes) uplo=uplo_of_epsboxes;

	cout << " best bound in: [" << loup << "," << uplo << "]" << endl;

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
