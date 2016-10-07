/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Ctc.h"
#include "ibex_SepCtcPair.h"
#include "ibex_CtcIdentity.h"

namespace ibex {

void Ctc::contract(IntervalVector& box, const BitSet& impact) {
	_impact = &impact;

	try {
		contract(box);
	}
	catch(Exception& e) { // ibex exceptions
		_impact = NULL;
		throw e;
	}
	catch (std::exception& e) { // other exceptions
		_impact = NULL;
		throw e;
	}
	catch (...) {
		ibex_error("contract: cannot handle exception");
	}

	_impact = NULL;
}


void Ctc::contract(IntervalVector& box, const BitSet& impact, BitSet& flags) {
	_impact = &impact;
	_output_flags = &flags;

	flags.clear();

	try {
		contract(box);
	}
	catch(Exception& e) { // ibex exceptions
		_impact = NULL;
		_output_flags = NULL;
		throw e;
	}
	catch (std::exception& e) { // other exceptions
		_impact = NULL;
		_output_flags = NULL;
		throw e;
	}
	catch (...) {
		ibex_error("contract: cannot handle exception");
	}

	_impact = NULL;
	_output_flags = NULL;
}

bool Ctc::check_nb_var_ctc_list (const Array<Ctc>& l)  {
	int i=1, n=l[0].nb_var;
	while (i<l.size() && l[i].nb_var==n) {
		i++;
	}
	return (i==l.size());
}

void Ctc::contract(Set& set, double eps) {
	CtcIdentity id(nb_var);
	SepCtcPair sep(id,*this);
	sep.contract(set,eps);
}



 bool Ctc::side_effects() {return _side_effects;}

  void Ctc::enable_side_effects()
  { //cout << " adding side effects " << endl; 
    _side_effects=true;}

  void Ctc::disable_side_effects()
  { //cout << " removing side effects " << endl; 
    _side_effects=false;}

} // namespace ibex
