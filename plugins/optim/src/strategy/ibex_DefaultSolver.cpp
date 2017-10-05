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

#include "ibex_LinearizerCombo.h"
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
#include "ibex_DefaultStrategy.cpp_"
#include "ibex_Random.h"
#include "ibex_NormalizedSystem.h"

using namespace std;

namespace ibex {

const double DefaultSolver::default_eps_x_min = 1e-3;
const double DefaultSolver::default_eps_x_max = POS_INFINITY;
const double DefaultSolver::default_random_seed = 1.0;

namespace {

// index in memory
//   -1=none (not built yet)
//   -2=no square sub-system of equations

int square_eq_system = -1;

System* get_square_eq_sys(System& sys) {
	if (square_eq_system>=0)
		return (*memory())->sys[square_eq_system]; // already built and recorded
	else if (square_eq_system==-1) {
		int nb_eq=0;

		// count the number of equalities
		for (int i=0; i<sys.nb_ctr; i++)
			if (sys.ctrs[i].op==EQ) nb_eq+=sys.ctrs[i].f.image_dim();

		if (sys.nb_var==nb_eq) {
			square_eq_system = (*memory())->sys.size();

			return &rec(new System(sys,System::EQ_ONLY));
		}
		else {
			square_eq_system=-2;
			return NULL; // not square
		}
	} else
		return NULL;
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

Ctc*  DefaultSolver::ctc (System& sys, double prec) {
	Array<Ctc> ctc_list(4);

	// first contractor : non incremental hc4
	ctc_list.set_ref(0, rec(new CtcHC4 (sys.ctrs,0.01)));
	// second contractor : acid (hc4)
	ctc_list.set_ref(1, rec(new CtcAcid (sys, rec(new CtcHC4 (sys.ctrs,0.1,true)))));
	int index=2;
	// if the system is a sqare system of equations, the third contractor is Newton
	System* eqs=get_square_eq_sys(sys);
	if (eqs) {
		ctc_list.set_ref(index,rec(new CtcNewton(eqs->f_ctrs,5e8,prec,1.e-4)));
		index++;
	}
	// the last contractor is XNewton
	//	ctc_list.set_ref(index,*new CtcXNewtonIter(sys,
	//                                          new CtcHC4 (sys.ctrs,0.01),
	//*(default_corners())));

	System& norm_sys=rec(new NormalizedSystem(sys));

	ctc_list.set_ref(index,rec(new CtcFixPoint(rec(new CtcCompo(
			rec(new CtcPolytopeHull(rec(new LinearizerCombo(norm_sys,LinearizerCombo::XNEWTON)))),
			rec(new CtcHC4 (sys.ctrs,0.01)))))));

	ctc_list.resize(index+1); // in case the system is not square.

	return new CtcCompo (ctc_list);
}


DefaultSolver::DefaultSolver(System& sys, double eps_x_min, double eps_x_max,
		bool dfs, double random_seed) : Solver(sys, rec(ctc(sys,eps_x_min)),
		get_square_eq_sys(sys)!=NULL?
				rec(new SmearSumRelative(*get_square_eq_sys(sys), eps_x_min)) :
				rec(new RoundRobin(eps_x_min)),
				rec(dfs? (CellBuffer*) new CellStack() : (CellBuffer*) new CellList()),
				Vector(sys.nb_var,eps_x_min), Vector(sys.nb_var,eps_x_max)),
		sys(sys) {

	RNG::srand(random_seed);

	data = *memory(); // keep track of my data

	*memory() = NULL; // reset (for next DefaultSolver to be created)

	square_eq_system = -1;
}

// Note: we set the precision for Newton to the minimum of the precisions.
DefaultSolver::DefaultSolver(System& sys, const Vector& eps_x_min, double eps_x_max,
		bool dfs, double random_seed) : Solver(sys, rec(ctc(sys,eps_x_min.min())),
		get_square_eq_sys(sys)!=NULL?
				rec(new SmearSumRelative(*get_square_eq_sys(sys), eps_x_min)) :
				rec(new RoundRobin(eps_x_min)),
		rec(dfs? (CellBuffer*) new CellStack() : (CellBuffer*) new CellList()),
		eps_x_min, Vector(sys.nb_var,eps_x_max)),
		sys(sys) {

	RNG::srand(random_seed);

	data = *memory(); // keep track of my data

	*memory() = NULL; // reset (for next DefaultSolver to be created)

	square_eq_system = -1;
}

DefaultSolver::~DefaultSolver() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}


} // end namespace ibex
