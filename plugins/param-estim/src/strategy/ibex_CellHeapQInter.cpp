//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeapQInter.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_CellHeapQInter.h"
#include "ibex_QInterPoints.h"

namespace ibex {

  CellHeapQInter::CellHeapQInter() : CellHeapGen()  {

}


double CellHeapQInter::cost(const Cell& c) const {
  return -(c.get<QInterPoints>().qmax );
  //  return -(c.get<QInterPoints>().qmax +0.001*c.get<QInterPoints>().depth );
  //   cout << c.get<QInterPoints>().qmax << " " << c.get<QInterPoints>().oracle << " " << c.get<QInterPoints>().depth << endl;
  //  return -(c.get<QInterPoints>().qmax +1000*c.get<QInterPoints>().oracle + 0.001*c.get<QInterPoints>().depth );
}


} // end namespace ibex
