/* ============================================================================
 * I B E X - ibex_ContractorList.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_ContractorList.h"

namespace ibex {

ContractorList::ContractorList(const vector<Contractor*>& vec) :
  nb_var(vec[0]->nb_var), _nb_ctc(vec.size()), ctcs(new Contractor*[vec.size()]) {

  int i=0;
  for (vector<Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
	assert((*it)->nb_var==nb_var);
    ctcs[i++] = *it;
  }
}

/*
ContractorList::ContractorList(const CtrToCtc& converter, const CSP& csp) :
  _nb_ctc(csp.nb_ctr()), ctcs(new Contractor*[csp.nb_ctr()]) {

  for (int i=0; i<csp.nb_ctr(); i++) {
    ctcs[i]=converter.convert(csp.ctr(i),csp.space);
  }
}
*/

ContractorList::ContractorList(Contractor& c1, Contractor& c2) :
  nb_var(c1.nb_var), _nb_ctc(2), ctcs(new Contractor*[2]) {
	assert(c1.nb_var==c2.nb_var);
    ctcs[0] = &c1;
    ctcs[1] = &c2;
}

ContractorList::ContractorList(ContractorList& list) :
  nb_var(list.nb_var), _nb_ctc(list.size()), ctcs(new Contractor*[list.size()]) {
  for (int i=0; i<_nb_ctc; i++) {
    ctcs[i] = &list(i);
  }
}

ContractorList::~ContractorList() {
  for (int i=0; i<size(); i++) {
    delete ctcs[i];
  }

  delete[] ctcs;
}

} // namespace ibex
