//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimSimplex.cpp
// Author      : Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================


#include "ibex_Optimizer.h"

#include <stdlib.h>
#include "soplex.h"

using namespace soplex;
using namespace std;

namespace ibex {

//The system is overconstrained by using the Taylor extension.
//Then the simplex algorithm is applied to obtain a new upper bound loup
//If a new loup is found the method returns true

bool Optimizer::update_loup_simplex(const IntervalVector& box) {

	IntervalVector G(n); // vector to be used by the partial derivatives

	IntervalVector x_inf(box.lb());

	IntervalVector g_inf(g.eval_vector(x_inf)); // may throw EmptyBoxException?

	// Interval f_inf=f.eval(x_inf);        // [gch] never used...

	SoPlex mysoplex;
	mysoplex.changeSense(SPxLP::MINIMIZE);	// set the objective sense

	DSVector dummycol(0);

	// ============================================================
	//                  linearize the objective
	// ============================================================
	f.gradient(box,G);

	//     if(G.max_diameter()>1e8) return false; //to avoid problems with SoPleX
	DSVector row1(n);

	//Linear variable yl is created
	//0 <= yl <= loup
	mysoplex.addCol(LPCol(1.0, dummycol, infinity, -infinity ));
	row1.add(0, -1.0);

	for (int j=0; j<n-1; j++){
		//The linear variables are generated
		//0 <= xl_j <= diam([x_j])
		mysoplex.addCol(LPCol(0.0, dummycol, box[j].diam(), 0.0));
		row1.add(j+1, G[j].ub());
	}
	//the first constraint:
	//sup(df/dx_1) xl_1 + ... + sup(df/dx_n) xl_n <= yl
	mysoplex.addRow(LPRow(-infinity, row1, 0.0));

	// ============================================================
	//                  linearize the constraints
	// ============================================================

	//The linear system is generated
	for (int i=0; i<m; i++) {

		if(g[i].eval(box).ub()<=0) continue;      // the constraint is satified :)

		g[i].gradient(box,G);                     // gradient calculation

		// ============================================================
		//     if(G.max_diameter()>1e8) return false; //to avoid problems with SoPleX
		DSVector row1(n);

		//The contraints i is generated:
		// c_i:  inf([g_i]([x]) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) + xl_n  <= -eps_error
		for (int j=0; j<n-1; j++) {
//			if(op == LEQ || op== LT) {
				row1.add(j+1, G[j].ub());
//			} else {
//				row1.add(j+1, Inf(G(j+1)));
//			}
		}

//		if(op == LEQ || op== LT) {
			mysoplex.addRow(LPRow(-infinity, row1, (-g_inf)[i].lb()-1e-10));    //  1e-10 ???  BNE
//		} else {
//			mysoplex.addRow(LPRow(Sup(-eval_inf(i+1))+1e-10, row1, infinity));
//		}
	}

	SPxSolver::Status stat;

	mysoplex.setTerminationIter(30);
	mysoplex.setDelta(1e-10);

	try{
		stat = mysoplex.solve();
	} catch(SPxException&) {
		return false;
	}

	if (stat == SPxSolver::OPTIMAL) {
		//the linear solution is mapped to intervals and evaluated
		DVector prim(n);
		mysoplex.getPrimal(prim);

		IntervalVector tmpbox(n);

		for (int j=0; j<n-1; j++)
			tmpbox[j]=box[j].lb()+prim[j+1];

		bool ret= check_candidate(box.mid(),false); // [gch] do we know here that the point is inner??

		if (ret) {
			int prec=cout.precision();
			cout.precision(12);
			cout << "[simplex1]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
			cout.precision(prec);
			nb_simplex++;
			diam_simplex= ((nb_simplex-1) * diam_simplex + box.max_diam()) / nb_simplex;
		}
		return ret;
	}

	if(stat == SPxSolver::ABORT_TIME) cout << "Simplex spent too much time" << endl;

	return false;
}

} // end namespace ibex
