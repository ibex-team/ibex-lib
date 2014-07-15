//============================================================================
//                                  I B E X                                   
// File        : ibex_3BCID.h
// Author      : Ignacio Araya, Gilles Chabert,
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 1, 2012
//============================================================================

#ifndef __IBEX_CTC_OPTIMSHAVING_H__
#define __IBEX_CTC_OPTIMSHAVING_H__

#include "ibex_Ctc3BCid.h"


namespace ibex {

  class CtcOptimShaving: public Ctc3BCid {
  public:
   CtcOptimShaving(Ctc& ctc, int s3b=default_s3b, int scid=default_scid,
			int vhandled=-1, double var_min_width=default_var_min_width);
   CtcOptimShaving(int nb_var, Ctc& ctc, int s3b=default_s3b, int scid=default_scid,
			int vhandled=-1, double var_min_width=default_var_min_width);
			
   virtual void contract(IntervalVector& box);
	static const int LimitCIDDichotomy;
  protected:
	virtual	bool var3BCID_dicho(IntervalVector& box, int var, double wv);
	virtual bool var3BCID_slices(IntervalVector& box, int var, int locs3b, double w_DC, Interval & dom);
	virtual int limitCIDDichotomy () ;
  };
  
	
} // end namespace ibex
#endif // __IBEX_CTC_OPTIMSAHVING_H__
