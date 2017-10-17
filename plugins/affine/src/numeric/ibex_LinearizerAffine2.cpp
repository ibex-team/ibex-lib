//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcART.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_LinearizerAffine2.h"

#include "ibex_Exception.h"

namespace ibex {

// the constructor
LinearizerAffine2::LinearizerAffine2(const System& sys1) :
				Linearizer(sys1.nb_var), sys(sys1),
				goal_af_evl(NULL),
				ctr_af_evl(new AffineEval<AF_Default>*[sys1.nb_ctr]) {

	if (sys1.goal) {
		goal_af_evl = new AffineEval<AF_Default>(*sys1.goal);
	}

	for (int i = 0; i < sys.nb_ctr; i++) {
		ctr_af_evl[i] = new AffineEval<AF_Default>(sys.ctrs[i].f);
	}
}

LinearizerAffine2::~LinearizerAffine2() {
	for (int i = 0; i < sys.nb_ctr; i++) {
		delete ctr_af_evl[i];
	}
	delete[] ctr_af_evl;
}

bool LinearizerAffine2::goal_linearization(const IntervalVector& box, LPSolver& lp_solver) {
	// Linearization of the objective function by AF2

	if (!sys.goal) {
		ibex_error("LinearRelaxAffine2: there is no goal function to linearize.");
	}

	goal_af_evl->eval(box);
	Affine2 af2 = goal_af_evl->af2.top->i();
	if (af2.is_empty()) {
		return false;
	}
	try {
		if (af2.size() == sys.nb_var) { // if the affine2 form is valid
			// convert the epsilon variables to the original box
			double tmp=0;
			for (int i =0; i <sys.nb_var; i++) {
				tmp = box[i].rad();
				if (tmp==0) { // sensible case to avoid rowconst[i]=NaN
					if (af2.val(i+1)==0)
						lp_solver.set_obj_var(i, 0);
					else {
						return false; // sensible case to avoid
					}
				} else {
					lp_solver.set_obj_var(i, af2.val(i+1) / tmp);
				}
			}
		}
		else {
			return false;
		}
		return true;

	} catch (LPException&) {
		return false;
	}
}


int LinearizerAffine2::inlinearization(const IntervalVector& box, LPSolver& lp_solver) {
	// TODO a verifier et finir

	Affine2 af2;

	int cont=0;
	Interval ev(0), center(0), err(0);
	Vector rowconst(sys.nb_var);

	// Create the linear relaxation of each constraint
	for (int ctr = 0; ctr < sys.nb_ctr; ctr++) {
		CmpOp op = sys.ctrs[ctr].op;
		ev  = ctr_af_evl[ctr]->eval(box).i();
		af2 = ctr_af_evl[ctr]->af2.top->i();

		//std::cout <<ev<<":::"<< af2<<"  "<<af2.size()<<"  " <<sys.nb_var<< std::endl;

		if (af2.size() == sys.nb_var) { // if the affine2 form is valid
			bool b_abort=false;
			// convert the epsilon variables to the original box
			double tmp=0;
			center =0;
			err =0;
			for (int i =0;(!b_abort) &&(i <sys.nb_var); i++) {
				tmp = box[i].rad();
				if (tmp==0) { // sensible case to avoid rowconst[i]=NaN
					if (af2.val(i+1)==0)
						rowconst[i]=0;
					else {
						b_abort =true;
					}
				} else {
					rowconst[i] =af2.val(i+1) / tmp;
					center += rowconst[i]*box[i].mid();
					err += fabs(rowconst[i])*  pow(2,-50);
				}
			}
			if (!b_abort) {
				switch (op) {
				case LEQ:
				case LT: {
					if (0.0 < ev.ub()) {
						try {// TODO TO CHECK
							lp_solver.add_constraint(rowconst, LEQ,	(-(af2.err()+err) - (af2.val(0)-center)).lb());
							cont++;
						} catch (LPException&) { }
					}
					break;
				}
				case GEQ:
				case GT: {
					if (ev.lb() < 0.0) {
						try {// TODO TO CHECK
							lp_solver.add_constraint(rowconst, GEQ,	((af2.err()+err) - (af2.val(0)-center)).ub());
							cont++;
						} catch (LPException&) { }
					}
					break;
				}
				case EQ: {
					not_implemented("LinearRelaxAffine2::inlinearization not implemented for equality constraints");
				}
				default:
					break;
				}
			}
		}

	}

	return -1;
}


/*********generation of the linearized system*********/
int LinearizerAffine2::linearize(const IntervalVector& box, LPSolver& lp_solver) {

	Affine2 af2;
	Vector rowconst(sys.nb_var);
	Interval ev(0.0);
	Interval center(0.0);
	Interval err(0.0);
	CmpOp op;
	int cont = 0;

	// Create the linear relaxation of each constraint
	for (int ctr = 0; ctr < sys.nb_ctr; ctr++) {

		op  = sys.ctrs[ctr].op;
		ev  = ctr_af_evl[ctr]->eval(box).i();
		af2 = ctr_af_evl[ctr]->af2.top->i();

		if (ev.is_empty()) {
			af2.set_empty();
		}
		//std::cout <<ev<<":::"<< af2<<"  "<<af2.size()<<"  " <<sys.nb_var<< std::endl;

		if (af2.size() == sys.nb_var) { // if the affine2 form is valid
			bool b_abort=false;
			// convert the epsilon variables to the original box
			double tmp=0;
			center =0;
			err =0;
			for (int i =0;(!b_abort) &&(i <sys.nb_var); i++) {
				tmp = box[i].rad();
				if (tmp==0) { // sensible case to avoid rowconst[i]=NaN
					if (af2.val(i+1)==0)
						rowconst[i]=0;
					else {
						b_abort =true;
					}
				} else {
					rowconst[i] =af2.val(i+1) / tmp;
					center += rowconst[i]*box[i].mid();
					err += fabs(rowconst[i])*  pow(2,-50);
				}
			}
			if (!b_abort) {
				switch (op) {
				case LT:
					if (ev.lb() == 0.0) return -1;
				case LEQ:
					if (0.0 < ev.lb()) return -1;
					else if (0.0 < ev.ub()) {
						try {
							lp_solver.add_constraint(rowconst, LEQ,	((af2.err()+err) - (af2.val(0)-center)).ub());
							cont++;
						} catch (LPException&) { }
					}
					break;
				case GT:
					if (ev.ub() == 0.0) return -1;
				case GEQ:
					if (ev.ub() < 0.0) return -1;
					else if (ev.lb() < 0.0) {
						try {
							lp_solver.add_constraint(rowconst, GEQ,	(-(af2.err()+err) - (af2.val(0)-center)).lb());
							cont++;
						} catch (LPException&) { }
					}
					break;
				case EQ:
					if (!ev.contains(0.0)) return -1;
					else {
						if (ev.diam()>2*lp_solver.get_epsilon()) {
							try {
								lp_solver.add_constraint(rowconst, GEQ,	(-(af2.err()+err) - (af2.val(0)-center)).lb());
								cont++;
								lp_solver.add_constraint(rowconst, LEQ,	((af2.err()+err) - (af2.val(0)-center)).ub());
								cont++;
							} catch (LPException&) { }
						}
					}
					break;
				}
			}
		}

	}
	return cont;

}

//void CtcART::convert_back(IntervalVector & box, IntervalVector & epsilon) {
//
//	for (int i = 0; i < box.size(); i++) {
//		box[i] &= box[i].mid() + (box[i].rad() * epsilon[i]);
//	}
//}

}
