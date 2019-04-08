//============================================================================
//                                  I B E X                                   
// File        : ibex_Multipliers.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2013
// Last update : Jun 07, 2018
//============================================================================

#include <stdlib.h>
#include "ibex_BxpMultipliers.h"

namespace ibex {

/*BxpMultipliers::BxpMultipliers(): lambda(1) {

}

void BxpMultipliers::init_root(int M, int R, int K) {

	lambda.resize(1+M+R+K);

	lambda[0]=Interval(0,1);

	for (int m=0; m<M; m++)
		lambda[1+m]=Interval(0,1);

	for (int r=0; r<R; r++)
		lambda[1+M+r]=Interval(-1,1);

	for (int k=0; k<K; k++)
		lambda[1+M+R+k]=Interval(0,1);
}


BxpMultipliers::BxpMultipliers(const BxpMultipliers& e) : lambda(e.lambda) {

}

Bxp* BxpMultipliers::copy() const {
	return new BxpMultipliers(*this);
}
*/
} // end namespace ibex
