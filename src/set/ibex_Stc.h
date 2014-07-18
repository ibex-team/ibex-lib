//============================================================================
//                                  I B E X                                   
// File        : ibex_Stc.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_STC_H__
#define __IBEX_STC_H__

namespace ibex {

class Stc {
public:
	Stc();
	virtual void contract(Set& set)=0;
	virtual ~Stc();
};

} // namespace ibex

#endif // __IBEX_STC_H__
