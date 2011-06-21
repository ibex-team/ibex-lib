/*---------------------------------------------------------------------------------
 * 
 * Paver Paving
 * -------------
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

#ifndef _IBEX_PAVING_H
#define _IBEX_PAVING_H

#include "IntervalVector.h"
#include "IbexSpace.h"
#include "IbexExpr.h"

namespace ibex {

class PavingNode;
class ContractorNode;
class BisectorNode;

class PavingVisitor {
 public:
  virtual void visit(const PavingNode&)=0;
  virtual void visit(const ContractorNode&)=0;
  virtual void visit(const BisectorNode&)=0;
};

/** \ingroup solvergroup
 * \brief Paving node
 * 
 * A paving is built by a call to a solver (see #ibex::Paver::explore()).
 * \author Gilles Chabert
 * \date January 2008
 */
class PavingNode { 
 public:
  virtual ~PavingNode() { }
  virtual void acceptVisitor(PavingVisitor&) const=0;
  PavingNode(const INTERVAL_VECTOR& box) : box(box) {}

  /** Return true if this node is a contractor node 
   * (so that a safe cast can be applied) and false
   * if it is a bisector node. */
  virtual bool is_contractor_node()=0;

  /** box of the node */
  INTERVAL_VECTOR box;

 private:  
};

ostream& operator<<(ostream&, const PavingNode& n);

class Paver;
//class Cell;

/** \ingroup solvergroup
 *  \brief Paving contractor node
 * 
 * A paving is built by a call to a solver (see #ibex::Paver::explore()).
 * \author Gilles Chabert
 * \date January 2008
 */
class ContractorNode : public PavingNode {
 public:

  ContractorNode(const INTERVAL_VECTOR& box, int number) : PavingNode(box), number(number), rej(NULL) {
  }

  virtual ~ContractorNode() { if (rej) delete rej; }

  /** Return true */
  bool is_contractor_node() { return true; }

  /** Return true if this node is a leaf in the paving. */
  bool leaf() const { return rej==NULL; }

  /** Return the rejected part by the contractor */
  const PavingNode& rejected() const { return *rej; }

  virtual void acceptVisitor(PavingVisitor& v) const { v.visit(*this); }

  /** Return the contractor number */
  int ctc_number() const { return number; }

 private:
  friend class Paver;
  int number;
  PavingNode* rej;
};


/** \ingroup solvergroup
 *  \brief Paving bisection node
 * 
 * A paving is built by a call to a solver (see #ibex::Paver::explore()).
 * \author Gilles Chabert
 * \date January 2008
 */
class BisectorNode : public PavingNode {
 public:

  BisectorNode(const INTERVAL_VECTOR& box) : PavingNode(box), l(NULL), r(NULL) {}

   /* Return true if left node exists. */ 
/*   bool exists_left() { l!=NULL; } */

/*   Return true if left node exists. */ 
/*   bool exists_right() { l!=NULL; } */

  /** Return true */
  bool is_contractor_node() { return false; }

  virtual ~BisectorNode() { 
    if (l) delete l; 
    if (r) delete r; 
  }

  /** Return the left part returned by the bisector */
  const PavingNode& left() const { return *l; }

  virtual void acceptVisitor(PavingVisitor& v) const { v.visit(*this); }

  /** Return the right part returned by the bisector */
  const PavingNode& right() const { return *r; }
  
 private:
  friend class Paver;
  PavingNode* l;
  PavingNode* r;
};


class PavingPrinter : public PavingVisitor {
 public:

  PavingPrinter(ostream& os, const PavingNode& n, int ctc=-1) : os(os), space(NULL), x(NULL), ctc(ctc) { 
    visit(n);
  }

  PavingPrinter(ostream& os, const PavingNode& n, Space& space, const ExtendedSymbol& x, int ctc=-1) : os(os), space(&space), x(&x), ctc(ctc) { 
    visit(n);
  }

 private:
  virtual void visit(const PavingNode& n);
  virtual void visit(const ContractorNode& c);
  virtual void visit(const BisectorNode& b);

  ostream& os;
  Space* space;
  const ExtendedSymbol* x;
  const int ctc;
};


} // end namespace

#endif
