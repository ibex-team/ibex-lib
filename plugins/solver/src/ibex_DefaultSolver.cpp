//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultSolver.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Nov 21, 2017
//============================================================================

#include "ibex_DefaultSolver.h"

#include "ibex_LinearizerXTaylor.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcNewton.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CellStack.h"
#include "ibex_CellList.h"
#include "ibex_Array.h"
#include "ibex_Random.h"
#include "ibex_NormalizedSystem.h"

using namespace std;

namespace ibex {

double DefaultSolver::default_eps_x_max = POS_INFINITY;

#define SQUARE_EQ_SYSTEM_TAG 1

namespace {

System* get_square_eq_sys(Memory& memory, System& sys) {
	if (memory.found(SQUARE_EQ_SYSTEM_TAG))
		return &memory.get<System>(SQUARE_EQ_SYSTEM_TAG);
	else {
		int nb_eq=0;

		// count the number of equalities
		// TODO: useless to do it every time get_square_eq_sys(...)
		// is called, when the system is not square
		for (int i=0; i<sys.nb_ctr; i++)
			if (sys.ctrs[i].op==EQ) nb_eq+=sys.ctrs[i].f.image_dim();

		if (sys.nb_var==nb_eq) {
			return &memory.rec(new System(sys,System::EQ_ONLY), SQUARE_EQ_SYSTEM_TAG);
		}
		else {
			return NULL; // not square
		}
	}
}

} // end namespace

// the corners for  Xnewton
/*std::vector<CtcXNewton::corner_point>*  DefaultSolver::default_corners () {
	std::vector<CtcXNewton::corner_point>* x;
	x= new std::vector<CtcXNewton::corner_point>;
	x->push_back(CtcXNewton::RANDOM);
	x->push_back(CtcXNewton::RANDOM_INV);
	return x;
}*/

Ctc* DefaultSolver::ctc (System& sys, double prec) {
	Array<Ctc> ctc_list(4);

	// first contractor : non incremental hc4
	ctc_list.set_ref(0, rec(new CtcHC4 (sys.ctrs,0.01)));
	// second contractor : acid (hc4)
	ctc_list.set_ref(1, rec(new CtcAcid (sys, rec(new CtcHC4 (sys.ctrs,0.1,true)))));
	int index=2;
	// if the system is a square system of equations, the third contractor is Newton
	System* eqs=get_square_eq_sys(*this, sys);
	if (eqs) {
		ctc_list.set_ref(index,rec(new CtcNewton(eqs->f_ctrs,5e8,prec,1.e-4)));
		index++;
	}

	//System& norm_sys=rec(new NormalizedSystem(sys));

	ctc_list.set_ref(index,rec(new CtcFixPoint(rec(new CtcCompo(
			rec(new CtcPolytopeHull(rec(new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN)))),
			rec(new CtcHC4 (sys.ctrs,0.01)))))));

	ctc_list.resize(index+1); // in case the system is not square.

	return new CtcCompo (ctc_list);
}

DefaultSolver::DefaultSolver(System& sys, double eps_x_min, double eps_x_max,
		bool dfs, double random_seed) : Solver(sys, rec(ctc(sys,eps_x_min)),
		get_square_eq_sys(*this, sys)!=NULL?
				(Bsc&) rec(new SmearSumRelative(*get_square_eq_sys(*this, sys), eps_x_min)) :
				(Bsc&) rec(new RoundRobin(eps_x_min)),
				rec(dfs? (CellBuffer*) new CellStack() : (CellBuffer*) new CellList()),
				Vector(sys.nb_var,eps_x_min), Vector(sys.nb_var,eps_x_max)),
		sys(sys) {

	RNG::srand(random_seed);

}

// Note: we set the precision for Newton to the minimum of the precisions.
DefaultSolver::DefaultSolver(System& sys, const Vector& eps_x_min, double eps_x_max,
		bool dfs, double random_seed) : Solver(sys, rec(ctc(sys,eps_x_min.min())),
		get_square_eq_sys(*this, sys)!=NULL?
				(Bsc&) rec(new SmearSumRelative(*get_square_eq_sys(*this, sys), eps_x_min)) :
				(Bsc&) rec(new RoundRobin(eps_x_min)),
		rec(dfs? (CellBuffer*) new CellStack() : (CellBuffer*) new CellList()),
		eps_x_min, Vector(sys.nb_var,eps_x_max)),
		sys(sys) {

	RNG::srand(random_seed);

}

} // end namespace ibex
