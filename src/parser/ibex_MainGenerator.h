//============================================================================
//                                  I B E X                                   
// File        : ibex_MainGenerator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#ifndef __IBEX_MAIN_GENERATOR_H__
#define __IBEX_MAIN_GENERATOR_H__

namespace ibex {

namespace parser {

class P_Source;
class P_Result;

class MainGenerator {
public:
	void generate(const P_Source& source, P_Result& result);
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_MAIN_GENERATOR_H__
