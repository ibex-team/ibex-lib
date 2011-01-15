/*---------------------------------------------------------------------------------
 * 
 * Constraint satisfaction problems
 * --------------------------------
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

#include "IbexEnv.h"
#include "IbexCSP.h"
#include "IbexContractor.h"
#include "IbexQuimper.h"

namespace ibex {

static const Constraint** vector_to_array(const vector<const Constraint*>& vec) {
  const Constraint** ctrs = new const Constraint*[vec.size()];
  for (unsigned int i=0; i<vec.size(); i++)
    ctrs[i] = vec[i];
  return ctrs;
}

static const Constraint** copy_array(const Constraint** arr, int size) {
  const Constraint** ctrs = new const Constraint*[size];
  for (int i=0; i<size; i++)
    ctrs[i] = arr[i];
  return ctrs;
}

CSP::CSP(const vector<const Constraint*>& ctrs, Space &space) :
  env(space.env), space(space), box(space.box), 
  ctrs(vector_to_array(ctrs)), _nb_ctr(ctrs.size())
{ 
  SharedObject::incr_ref(space.id);
  SharedObject::incr_ref(space.env.id);
}

CSP::CSP(const vector<const Constraint*>& ctrs, SpaceFactory &fac) :
  env(fac.env), space(*new Space(fac)), box(space.box), 
  ctrs(vector_to_array(ctrs)), _nb_ctr(ctrs.size())
{ 
  SharedObject::incr_ref(fac.env.id);
}

CSP::CSP(SpaceFactory &fac) :
  env(fac.env), space(*new Space(fac)), box(space.box),
  ctrs(new const Constraint*[env.nb_constraints()]), _nb_ctr(env.nb_constraints())
{ 
  SharedObject::incr_ref(fac.env.id);
  for (int i=0; i<env.nb_constraints(); i++)
    ctrs[i] = &env.constraint(i);
}

/*--------------------------------------------------------------------------------*/
/* If, as in Java, we could call another constructor of the same class in C++
 * then the following constructor would simply be: 
 *   CSP::CSP(const char* filename) : CSP(QuimperParser(filename)) { }
 * Since such invokation is unfortunately invalid, we have to define the following very ugly static variable:  */
static QuimperParser* _parser;
/*--------------------------------------------------------------------------------*/

/* Note: in (ANSI?) C++, initialization is done in the following order:
 *   1- virtual base classes, 
 *   2- base classes,
 *   3- member data. 
 * Hence, parser and space.box are referenced properly. */
CSP::CSP(const char* filename) :
  env(*(_parser = new QuimperParser(filename))->env), space(*(_parser->space)), box(space.box)
{ 
  bool has_constraints = _parser->constraints;

  if (has_constraints) {
    _nb_ctr = _parser->constraints->size();
    ctrs = vector_to_array(*_parser->constraints);
  }

  delete _parser;    

  if (!has_constraints) throw SyntaxErrorException("\"constraints\" keyword required");
    
}

CSP::CSP(const CSP& csp) :
  env(csp.env), space(csp.space), box(space.box),
  ctrs(copy_array(csp.ctrs, csp.nb_ctr())), 
  _nb_ctr(csp.nb_ctr())
{ 
  SharedObject::incr_ref(space.id);
  SharedObject::incr_ref(space.env.id);
}

CSP::~CSP() { 

  /* the system may have been built in several ways:
   * - the space does not belong to the system
   * - the space belongs to the system. It must be
   *    deleted with the system
   * - the system is built from a parser.
   *    In this case, both the environment and the space belong to the system
   *    and must be deleted with the system. */
  delete[] ctrs;

  SharedObject::decr_ref(space.env.id);

  SharedObject::decr_ref(space.id);
}

ostream& operator<<(ostream& os, const CSP& csp) {
  for (int i=0; i<csp.nb_ctr(); i++) 
    os << csp.ctr(i) << endl;
  return os;
}

} // end namespace
