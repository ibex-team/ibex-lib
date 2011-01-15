/*---------------------------------------------------------------------------------
 * 
 * Operator pretty printer
 * ----------------------
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


#include "IbexOperatorPrinter.h"
#include "IbexOperator.h"
#include "IbexBisector.h"
#include "IbexContractor.h"
#include "IbexNewton.h"
#include "IbexSequence.h"
#include "IbexUnivNewton.h"
#include "IbexBoxNarrow.h"
#include "IbexNVector.h"
#include "IbexPropagation.h"
#include "IbexHC4.h"
#include "IbexShaving.h"
#include "Ibex3BCID.h"
#include "IbexConstraint.h"
#include "IbexPaver.h"
#include "IbexCertification.h"

using std::endl;

namespace ibex {

void OperatorPrinter::print(const Operator& op) const { 
  op.accept_printer(*this);
}

void OperatorPrinter::print(const Contractor& op) const { 
  op.accept_printer(*this);
}

void OperatorPrinter::print(const ContractorList& ctrs, int logic_op) const {
  for (int i=0; i<ctrs.size(); i++) {
    if (i>0) { 
      for (int j=0; j<shift; j++) os << " ";
      switch(logic_op) {
      case 1  : os << "^ "; break;
      case -1 : os << "v "; break;
      case 0  : os << "  "; break;
      } 
    }
    else os << "  ";
    shift+=2;
    print(ctrs(i));
    os << endl;
    shift-=2;
  }      
}

  /*void OperatorPrinter::print(const ConstraintList& ctrs) const {
  for (int i=0; i<ctrs.size(); i++) {
    for (int j=0; j<shift; j++) os << " ";
    print(ctrs(i));
  }
  }*/

void OperatorPrinter::print(const Newton& newt) const { 
  os << "NEWTON ";
  shift+=7;
  for (int i=0; i<newt.sys.nb_ctr(); i++) 
    print(newt.sys.ctr(i));
  shift-=7;
}

void OperatorPrinter::print(const Sequence& csp) const {
  print(csp.list, csp.logic_op? 1 : -1);
}

void OperatorPrinter::print(const BoxNarrow& narrow) const { 
  os << "[box] ";
  print(narrow.equ);
}

void OperatorPrinter::print(const Slicer& psplit) const { 
  os << "[param-split] ";
  print(psplit.ctc);
}

void OperatorPrinter::print(const Propagation& propag) const { 
  os << "PROPAG ";
  shift+=7;
  print(propag.list, 0);
  shift-=7;
}

void OperatorPrinter::print(const HC4Revise& hc4) const { 
  print(hc4.ctr);
}

void OperatorPrinter::print(const HC4& hc4) const { 
  os << "HC4 ";
  shift+=4;
  print(hc4.list, 0);
  shift-=4;
}

void OperatorPrinter::print(const Shaving& shave) const { 
  os << "SHAVE ";
  shift+=6;
  print(shave.ctc);
  shift-=6;
}

void OperatorPrinter::print(const _3BCID& m3Bcid) const { 
  os <<"3BCID ";
  shift+=6;
  print(m3Bcid.ctc);
  shift-=6;
}

void OperatorPrinter::print(const Precision& p) const {
  os << "precision(" << p.ceil << ")" << endl;
}

void OperatorPrinter::print(const Certification& c) const {
  os << "certify(";
  print(*c.prover); 
  os << ",";
  print(*c.funnel);
  os << "," << c.ceil << ")" << endl;
}

/*--------------------------------------------------------------------------------*/

void OperatorPrinter::print(const RoundRobin& bisector) const {
  os << "round-robin bisector\n";
}

void OperatorPrinter::print(const LargestFirst& bisector) const {
  os << "largest-first bisector\n";
}

void OperatorPrinter::print(const SmearMax& bisector) const {
  os << "smear function with max aggregation bisector\n";
}

void OperatorPrinter::print(const SmearSum& bisector) const {
  os << "smear function with sum aggregation bisector\n";
}

void OperatorPrinter::print(const BlockRoundRobin& bisector) const {
  os << "blockwise round-robin bisector\n";
}

/*--------------------------------------------------------------------------------*/

void OperatorPrinter::print(const Constraint& ctr) const {
  ctr.accept_printer(*this);
}

void OperatorPrinter::print(const Equality& equ) const {
  os << equ.expr << "=0";
}

void OperatorPrinter::print(const Inequality& ineq) const {
  os << ineq.expr << ineq.op << "0";
}

void OperatorPrinter::print(const AtMostNVector& a) const {
  os << "atmost-nvector({";
  for (int i=0; i<a.nb_expr()-1; i++) 
    os << a.expr(i) << ',';
  if (a.nb_expr()>0) os << a.expr(a.nb_expr()-1);
  os << "}," << a.N << ")";
}

void OperatorPrinter::print(const NotIn& ni) const {
  os << ni.left << " not-in " << ni.right;
}

}
