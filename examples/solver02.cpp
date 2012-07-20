#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

	// Example #17
	// ------------------------------------------------
	// Solver (with a function loaded from a file)
	//
	// Solve a system using interval Newton and constraint
	// propagation.
	// ------------------------------------------------


	// Load a system of equations
	// --------------------------
	try {

	if (argc<2) {
		cerr << "usage: solver2 [filename]" << endl;
		exit(1);
	}

	System sys(argv[1]);

	// Build contractor #1:
	// --------------------------
	// A "constraint propagation" loop.
	// Each constraint in sys.ctrs is an
	// equation.
	CtcHC4 hc4(sys.ctrs,0.1);
	hc4.accumulate=true;

	// Build contractor #2:
	// --------------------------
	// An interval Newton iteration
	// for solving f(x)=0 where f is
	// a vector-valued function representing
	// the system.
	CtcNewton newton(sys.f);

	// Build the main contractor:
	// ---------------------------
	// A composition of the two previous
	// contractors
	CtcCompo c(hc4,newton);

	// Build the bisection heuristic
	// --------------------------
	// Round-robin means that the domain
	// of each variable is bisected in turn
	RoundRobin rr;

	// Chose the way the search tree is explored
	// -------------------------------------
	// A "CellStack" means a depth-first search.
	CellStack buff;

	// Build a solver
	// -------------
	// The last parameter (1e-07) is the precision
	// required for the solutions
	Solver s(c,rr,buff,1e-07);

	//s.trace=true;

	// Get the solutions
	vector<IntervalVector> sols=s.solve(sys.box);

	// Display the number of solutions
	cout << "number of solutions=" << sols.size() << endl;

	// Display the number of boxes (called "cells")
	// generated during the search
	cout << "number of cells=" << s.nb_cells << endl;

	// Display the solutions
	//
	for (int i=0; i< sols.size() ; i++)
		cout << " sol  "<< i+1 << " : " << sols[i] << endl;

	} catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}

	// ------------------------------------------------
}



