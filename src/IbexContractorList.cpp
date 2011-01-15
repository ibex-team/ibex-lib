/*---------------------------------------------------------------------------------
 * 
 * Contractor list
 * ---------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#include "IbexContractorList.h"
#include "IbexEnv.h"
#include "IbexCSP.h"

namespace ibex {

ContractorList::ContractorList(const vector<const Contractor*>& vec) :
  _nb_ctc(vec.size()), ctcs(new Contractor*[vec.size()]) { 

  int i=0;
  for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
    ctcs[i++] = (*it)->copy();
  }
}


ContractorList::ContractorList(const CtrToCtc& converter, const CSP& csp) :
  _nb_ctc(csp.nb_ctr()), ctcs(new Contractor*[csp.nb_ctr()]) {  

  for (int i=0; i<csp.nb_ctr(); i++) {
    ctcs[i]=converter.convert(csp.ctr(i),csp.space);
  } 
}

ContractorList::ContractorList(const Contractor& c1, const Contractor& c2) :
  _nb_ctc(2), ctcs(new Contractor*[2]) {
    ctcs[0] = c1.copy();
    ctcs[1] = c2.copy();
}

ContractorList::ContractorList(const ContractorList& list) : 
  _nb_ctc(list.size()), ctcs(new Contractor*[list.size()]) {
  for (int i=0; i<_nb_ctc; i++) {
    ctcs[i] = list(i).copy();
  }
}

ContractorList::~ContractorList() {
  for (int i=0; i<size(); i++) {
    delete ctcs[i];
  }

  delete[] ctcs;
}

} // end namespace
