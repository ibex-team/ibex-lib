//============================================================================
//                                  I B E X                                   
// File        : data.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 5, 2013
// Last Update : Jun 5, 2013
//============================================================================


#ifndef __DATA_H__
#define __DATA_H__

#include "ibex.h"

using namespace ibex;
//! [data]
/*================================ data ==================================*/
extern const int N;               // number of beacons
extern const int T;               // number of time steps
extern const double L;            // limit of the environment (the
                                  // robot is in the area [0,L]x[0,L])
extern const int NB_OUTLIERS;     // maximal number of outliers per
                                  // time units
extern IntervalMatrix beacons;    // (a Nx2 matrix) beacons[i] is the
                                  // position (x,y) of the ith beacon

extern IntervalMatrix d;          // (a TxN matrix) d[t][i]=distance
                                  // between x[t] and the ith beacon

extern IntervalMatrix v;          // (a Tx2 matrix) v[t] is the delta
				  // vector between x[t+1] and x[t].
/*========================================================================*/
//! [data]
void init_data();

#endif // __DATA_H__
