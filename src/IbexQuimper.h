/*---------------------------------------------------------------------------------
 * 
 * Quimper 
 * -------------------------------
 *
 * Copyright (C) 2009 Gilles Chabert
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

#ifndef _IBEX_QUIMPER_H
#define _IBEX_QUIMPER_H

#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexIdTable.h"
#include "IbexContractor.h"

namespace ibex {

/** \brief Quimper parser
 *
 *
 * \author Gilles Chabert
 * \date July 2009 
 * */

class QuimperParser {
 public:

  /** Parse a file and build Quimper objects */
  QuimperParser(const char* filename);

  ~QuimperParser();

//  friend CSP::CSP(const char*);

  /** The environment */
  Env* env;

  /** The space */
  Space* space;

  /** The constraints */
  const vector<const Constraint*>* constraints;

  /** The contractors */
  const IdTable<Contractor*> contractors;
  
 private:
  static void clean();
};

/** \brief Quimper system
 *
 * \author Gilles Chabert
 * \date July 2009 
 * */
class Quimper {
 public:
  Quimper(const char* filename);

  void contract(const char* ctc);

  void set_domain(const char* id, double lb, double ub);

  void set_var_domain(int i, double lb, double ub);

  void set_syb_domain(int i, double lb, double ub);

  double get_lb(const char* id);

  double get_ub(const char* id);

  double get_var_lb(int i);

  double get_var_ub(int i);

  double get_syb_lb(int i);

  double get_syb_ub(int i);

 private:
  QuimperParser parser;
};


}

#endif
