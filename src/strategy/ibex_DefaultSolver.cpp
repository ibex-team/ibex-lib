//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultSolver.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Jul 06, 2014
//============================================================================

#include "ibex_DefaultSolver.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcNewton.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CellStack.h"
#include "ibex_LinearRelaxCombo.h"
#include "ibex_Array.h"
#include "ibex_DefaultStrategy.cpp_"

using namespace std;

namespace ibex {

/* patch */
bool square_eq_sys(const System& sys) {
	if (sys.nb_var!=sys.nb_ctr) return false;
	for (int i=0; i<sys.nb_ctr; i++)
		if (sys.ctrs[i].op!=EQ) return false;
	return true;
}

// the corners for  Xnewton
/*std::vector<CtcXNewton::corner_point>*  DefaultSolver::default_corners () {
	std::vector<CtcXNewton::corner_point>* x;
	x= new std::vector<CtcXNewton::corner_point>;
	x->push_back(CtcXNewton::RANDOM);
	x->push_back(CtcXNewton::RANDOM_INV);
	return x;
}*/

Ctc*  DefaultSolver::ctc (System& sys, double prec) {
	Array<Ctc> ctc_list(4);

	// first contractor : non incremental hc4
	ctc_list.set_ref(0, rec(new CtcHC4 (sys.ctrs,0.01)));
	// second contractor : acid (hc4)
	ctc_list.set_ref(1, rec(new CtcAcid (sys, rec(new CtcHC4 (sys.ctrs,0.1,true)))));
	int index=2;
	// if the system is a square system of equations, the third contractor is Newton
	if (square_eq_sys(sys)) {
		ctc_list.set_ref(index,rec(new CtcNewton(sys.f,5e8,prec,1.e-4)));
		index++;
	}
	// the last contractor is XNewton
	//	ctc_list.set_ref(index,*new CtcXNewtonIter(sys,
	//                                          new CtcHC4 (sys.ctrs,0.01),
	//*(default_corners())));

	ctc_list.set_ref(index,rec(new CtcFixPoint(rec(new CtcCompo(
			rec(new CtcPolytopeHull(rec(new LinearRelaxCombo(sys,LinearRelaxCombo::XNEWTON)),CtcPolytopeHull::ALL_BOX)),
			rec(new CtcHC4 (sys.ctrs,0.01)))))));

	ctc_list.resize(index+1); // in case the system is not square.

	return new CtcCompo (ctc_list);
}


DefaultSolver::DefaultSolver(System& sys, double prec) : Solver(rec(ctc(sys,prec)),
		rec(new SmearSumRelative(sys, prec)),
		rec(new CellStack())),
		sys(sys) {

	srand(1);

	data = *memory(); // keep track of my data

	*memory() = NULL; // reset (for next DefaultSolver to be created)
}


DefaultSolver::~DefaultSolver() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}


} // end namespace ibex
