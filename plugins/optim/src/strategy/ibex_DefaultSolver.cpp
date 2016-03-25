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
#include "ibex_Random.h"

using namespace std;

namespace ibex {

namespace {

System* square_eq_sys(System& sys) {

	int nb_eq=0;

	// count the number of equalities
	for (int i=0; i<sys.nb_ctr; i++)
		if (sys.ctrs[i].op==EQ) nb_eq+=sys.ctrs[i].f.image_dim();

	if (sys.nb_var==nb_eq)
		if (nb_eq==sys.f.image_dim())
			return &sys; // useless to create a new one
		else {
			return &rec(new System(sys,System::EQ_ONLY));
		}
	else
		return NULL; // not square
}

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
	// if the system is a sqare system of equations, the third contractor is Newton
	System* eqs=square_eq_sys(sys);
	if (eqs) {
		ctc_list.set_ref(index,rec(new CtcNewton(eqs->f,5e8,prec,1.e-4)));
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

	RNG::srand(1);

	data = *memory(); // keep track of my data

	*memory() = NULL; // reset (for next DefaultSolver to be created)
}

// Note: we set the precision for Newton to the minimum of the precisions.
DefaultSolver::DefaultSolver(System& sys, const Vector& prec) : Solver(rec(ctc(sys,prec.min())),
		rec(new SmearSumRelative(sys, prec)),
		rec(new CellStack())),
		sys(sys) {

	RNG::srand(1);

	data = *memory(); // keep track of my data

	*memory() = NULL; // reset (for next DefaultSolver to be created)
}

DefaultSolver::~DefaultSolver() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}


} // end namespace ibex
