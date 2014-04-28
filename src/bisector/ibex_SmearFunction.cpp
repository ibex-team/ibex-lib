//============================================================================
//                               I B E X                                   
// File        : ibex_SmearFunction.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 19 2012
// Last Update : July 19 2012
//============================================================================

#include "ibex_SmearFunction.h"

using std::pair;
using namespace std;

namespace ibex {


pair<IntervalVector,IntervalVector> SmearFunction::bisect(const IntervalVector& box, int& last_var) {
	IntervalMatrix J(sys.nb_ctr, sys.nb_var);

	sys.f.jacobian(box,J);
	// in case of infinite derivatives  changing to roundrobin bisection
	for (int i=0;i < sys.nb_ctr;i++)
		for (int j=0;j < sys.nb_var;j++)
			if (J[i][j].mag() == POS_INFINITY ||((J[i][j].mag() ==0) && box[j].diam()== POS_INFINITY ))
				return RoundRobin::bisect(box,last_var);
	int var = var_to_bisect (J,box);
	// in case of selected var with infinite domain, change to round-robin bisection
	if (var == -1 || !(box[var].is_bisectable()))
		return RoundRobin::bisect(box,last_var);
	else
		return box.bisect(var,ratio);
}

// computes the variable with the greatest maximal impact
int SmearMax::var_to_bisect (IntervalMatrix& J, const IntervalVector& box) const {
	double max_magn = NEG_INFINITY;
	int var=-1;
	for (int j=0; j<nbvars; j++) {
		if ((!too_small(box,j)) && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
			for (int i=0; i<sys.nb_ctr; i++) {
				if ( J[i][j].mag() * box[j].diam() > max_magn ) {
					max_magn = J[i][j].mag()* box[j].diam();
					var = j;
				}
			}
		}
	}

	return var;
}


// computes the variable with the greatest  sum of impacts
int SmearSum::var_to_bisect(IntervalMatrix& J,const IntervalVector& box) const {
	double max_magn = NEG_INFINITY;
	int var = -1;

	for (int j=0; j<nbvars; j++) {
		if ((!too_small(box,j)) && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
			double sum_smear=0;
			for (int i=0; i<sys.nb_ctr; i++) {
				sum_smear+= J[i][j].mag() *box[j].diam();
			}
			if (sum_smear > max_magn) {
				max_magn = sum_smear;
				var = j;
			}
		}
	}
	return var;
}


int SmearSumRelative::var_to_bisect(IntervalMatrix & J, const IntervalVector& box) const {
	double max_magn = NEG_INFINITY;
	int var = -1;
	// the normalizing factor per constraint
	double* ctrjsum = new double[sys.nb_ctr];

	for (int i=0; i<sys.nb_ctr; i++) {
		ctrjsum[i]=0;
		for (int j=0; j<nbvars ; j++) {
			ctrjsum[i]+= J[i][j].mag() * box[j].diam();
		}
	}
	// computes the variable with the maximal sum of normalized impacts
	for (int j=0; j<nbvars; j++) {
		if ((!too_small(box,j)) && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j))) {
			double sum_smear=0;
			for (int i=0; i<sys.nb_ctr; i++) {
				if (ctrjsum[i]!=0)
					sum_smear+= J[i][j].mag() * box[j].diam() / ctrjsum[i];
			}
			if (sum_smear > max_magn) {
				max_magn = sum_smear;
				var = j;
			}
		}
	}
	delete[] ctrjsum;
	return var;
}

int SmearMaxRelative::var_to_bisect(IntervalMatrix & J,const IntervalVector& box) const {

	double max_magn = NEG_INFINITY;
	int var = -1;

	double* ctrjsum = new double[sys.nb_ctr]; // the normalizing factor per constraint
	for (int i=0; i<sys.nb_ctr; i++) {
		ctrjsum[i]=0;
		for (int j=0; j<nbvars ; j++) {
			ctrjsum[i]+= J[i][j].mag() * box[j].diam() ;
		}
	}

	// computes the variable with the greatest normalized impact
	double maxsmear=0;
	for (int j=0; j<nbvars; j++) {
		if ((!too_small(box,j)) && (box[j].mag() <1 ||  box[j].diam()/ box[j].mag() >= prec(j)))

			for (int i=0; i<sys.nb_ctr; i++) {
				if (ctrjsum[i]!=0)
					maxsmear = J[i][j].mag() * box[j].diam() / ctrjsum[i];
				if (maxsmear > max_magn) {
					max_magn = maxsmear;
					var = j;
				}
			}
	}
	delete[] ctrjsum;
	return var;
}


} // end namespace ibex
