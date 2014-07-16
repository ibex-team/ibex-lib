//============================================================================
//                                  I B E X
// File        : doc-contractor.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2014
// Last Update : Jun 03, 2014
//============================================================================


#include "ibex.h"
#include <fstream>

using namespace std;
using namespace ibex;

/**
 * This file contains examples of the documentation.
 */

class LinearSystem : public LinearRelax {
public:
	LinearSystem(const Matrix& A, const Vector& b) : LinearRelax(2,2), A(A), b(b) { }

	virtual int linearization(IntervalVector & box, LinearSolver *lp_solver)  {
		for (int i=0; i<A.nb_rows(); i++)
	lp_solver->addConstraint(A[i],LEQ,b[i]);
		return A.nb_rows();
	}

	Matrix A;
	Vector b;

};


//! [ctc-propag-1-C]
class Count : public CtcFwdBwd {
public:
	static int count;

	Count(const NumConstraint& ctr) : CtcFwdBwd(ctr) { }

	void contract(IntervalVector& box) {
		CtcFwdBwd::contract(box);
		count++;
	}
};


int Count::count=0;
//! [ctc-propag-1-C]

int main() {

	ofstream output;
	output.open ("doc-contractor.txt");

	output << "================= this file is generated ==============" << endl;

	{
	output << "! [ctc-propag-O]" << endl;
	//! [ctc-propag-2-C]

	// Load a system of constraints
	System sys("../benchs/benchs-satisfaction/benchs-coprin/DiscreteBoundary-0100.bch");

	// The array of contractors we will use
	Array<Ctc> ctc(sys.nb_ctr);

	for (int i=0; i<sys.nb_ctr; i++)
		// Create contractors from constraints and store them in "ctc"
		ctc.set_ref(i,*new Count(sys.ctrs[i]));
	//! [ctc-propag-2-C]


	//! [ctc-propag-3-C]
	double prec=1e-03;            // Precision upto which we calculate the fixpoint
	//! [ctc-propag-3-C]

	//! [ctc-propag-4-C]
	// =============================== with simple fixpoint ==============================
	Count::count=0;               // initialize the counter

	CtcCompo compo(ctc);          // make the composition of all contractors
	CtcFixPoint fix(compo,prec);  // make the fixpoint

	IntervalVector box=sys.box;   // tested box (load domains written in the file)

	fix.contract(box);

	output << " Number of contractions with simple fixpoint=" << Count::count << endl;
	//! [ctc-propag-4-C]

	//! [ctc-propag-5-C]
	// ================================= with propagation =================================
	Count::count=0;                            // initialize the counter

	CtcPropag propag(ctc, prec);   // Propagation of all contractors

	IntervalVector box2=sys.box;               // tested box (load domains written in the file)

	propag.contract(box2);

	output << " Number of contractions with propagation=" << Count::count << endl;

	output << " Are the results the same? " << (box.rel_distance(box2)<prec? "YES" : "NO") << endl;

	//! [ctc-propag-5-C]

	output << "! [ctc-propag-O]" << endl;

	}

	{
	output << "! [ctc-inv-O]" << endl;
	//! [ctc-inv-C]

	// Build a contractor on R² wrt (x>=0 and y>=0).

	Function gx("x","y","x");   // build (x,y)->x
	Function gy("x","y","y");   // build (x,y)->y

	NumConstraint geqx(gx,GEQ); // build x>=0
	NumConstraint geqy(gy,GEQ); // build y>=0

	CtcFwdBwd cx(geqx);
	CtcFwdBwd cy(geqy);

	CtcCompo compo(cx,cy);     // final contractor wrt (x>=0, y>=0)

	// Build a mapping from R to R²
	Function f("t","(cos(t),sin(t))");

	// Build the inverse contractor
	CtcInverse inv(compo,f);

	double pi=3.14;
	IntervalVector box(1,Interval(0,2*pi));

	inv.contract(box);
	output << "contracted box (first time):" << box << endl;

	inv.contract(box);
	output << "contracted box (second time):" << box << endl;

	//! [ctc-inv-C]
	output << "! [ctc-inv-O]" << endl;
	}

	{
	//! [ctc-polytope-hull]

	// build the matrix
	double _A[4]= {1,1,1,-1};
	Matrix A(2,2,_A);

	// build the vector
	Vector b=Vector::zeros(2);

	// create the linear system
	LinearSystem sys(A,b);

	// create the contractor wrt the linear system
	CtcPolytopeHull ctc(sys);

	// create a box
	IntervalVector box(2,Interval(-1,1));

	// contract it
	output << "box before contraction=" << box << endl;
	ctc.contract(box);
	output << "box after contraction=" << box << endl;

	//! [ctc-polytope-hull]
	}
}
