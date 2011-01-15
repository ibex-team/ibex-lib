/*---------------------------------------------------------------------------------
 * 
 * Contractor expression
 * ---------------------
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

#ifndef __PARSER_CTC_H
#define __PARSER_CTC_H

#include "IbexExpr.h"
#include "IbexContractor.h"
#include "ParserConstraint.h"

/* the following includes just for default values of parameters */
#include "IbexPropagation.h"
#include "IbexShaving.h"
#include "IbexNewton.h"
#include "IbexBoxNarrow.h"
#include "IbexUnivNewton.h"

namespace ibex {

namespace parser {


class P_Ctc;
class P_CtcRef;
class P_Shave;
class P_Newton;
class P_Propag;
class P_HC4Revise;
class P_BoxNarrow;
class P_ParamSplit;
class P_Precision;
class P_LogicOp;
class P_LogicOpLoop;
class P_QInter;
class P_Load;

class P_CtcVisitor {
 public:
  virtual void visit(const P_Ctc&)=0;
  virtual void visit(const P_CtcRef&)=0;
  virtual void visit(const P_Shave&)=0;
  virtual void visit(const P_Newton&)=0;
  virtual void visit(const P_Propag&)=0;
  virtual void visit(const P_HC4Revise&)=0;
  virtual void visit(const P_BoxNarrow&)=0;
  virtual void visit(const P_ParamSplit&)=0;
  virtual void visit(const P_Precision&)=0;
  virtual void visit(const P_LogicOp&)=0;
  virtual void visit(const P_LogicOpLoop&)=0;
  virtual void visit(const P_QInter&)=0;
  virtual void visit(const P_Load&)=0;
};

/* Base class of contractor expressions. */
class P_Ctc : public P_Blk<P_Ctc> { 
 public:
  virtual void accept(P_CtcVisitor& v) const=0;

  virtual void accept(P_BlkVisitor<P_Ctc>& v) const {
    v.visit(*this);
 }

  virtual ~P_Ctc() { }
};

ostream& operator<<(ostream& os, const P_Ctc& e);

/* A reference to the element of a contractor list */
class P_CtcListElemRef : public P_Ctc {
 public:
  P_CtcListElemRef(const char* name, const P_IterExpr& iter) : name(strdup(name)), iter(iter) { }

  ~P_CtcListElemRef() { 
    free((char*) name); 
    delete &iter;
  }

  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); }

  const char* name;
  const P_IterExpr& iter;
};

/* A reference to a contractor. */
class P_CtcRef : public P_Ctc {
 public:
  P_CtcRef(const char* name) : name(strdup(name)) { }


  ~P_CtcRef() { 
    free((char*) name); 
  }

  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); }

  const char* name;
};

/* Base class of all built-in operators on contractors. */  
class P_CtcOp : public P_Ctc {
 public:
  P_CtcOp(const P_Ctc& ctc) : ctc(ctc) { }

  ~P_CtcOp() { delete &ctc; }

  const P_Ctc& ctc;
};

/* Composite operation */
class P_CompOp : public P_Ctc {
 public:
  P_CompOp(const P_List<P_Ctc>& list) : list(list) { }

  P_CompOp(const P_Blk<P_Ctc>& c1, const P_Blk<P_Ctc>& c2) : list(*new P_BlkList<P_Ctc>()) { 
    ((P_BlkList<P_Ctc>&) list).add(c1);
    ((P_BlkList<P_Ctc>&) list).add(c2);
  }

  ~P_CompOp() { delete &list; }

  const P_List<P_Ctc>& list;
};

/* System operation */
class P_SysOp : public P_Ctc {
 public:
  P_SysOp(const P_List<P_Ctr>& sys) : sys(sys) { }

  ~P_SysOp() { delete &sys; }

  const P_List<P_Ctr>& sys;
};

/* Equality operator */
class P_CtrOp : public P_Ctc {
 public:
  P_CtrOp(const P_Ctr& ctr) : ctr(ctr) { }

  ~P_CtrOp() { delete &ctr; }

  const P_Ctr& ctr;
};

/* Shaving */
class P_Shave : public P_CtcOp {
 public:
  P_Shave(const P_Ctc& ctc, REAL ratio=ibex::Shaving::default_ratio, REAL var_ratio=ibex::Shaving::default_var_ratio) : P_CtcOp(ctc),
    ratio(ratio), var_ratio(var_ratio) { }
  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); }
  REAL ratio, var_ratio;
};

/* Parameter splitting operator */
class P_ParamSplit : public P_CtcOp {
 public:
  P_ParamSplit(const P_Ctc& ctc, const P_ExtendedSymbol& x, int nb_slices, bool logic_op) : 
    P_CtcOp(ctc), x(x), nb_slices(nb_slices), logic_op(logic_op) { }

  ~P_ParamSplit() { delete &x; }

  void accept(P_CtcVisitor& v) const { v.visit(*this); }

  const P_ExtendedSymbol& x;
  const int nb_slices;
  const bool logic_op;
};


/* Newton operation */
class P_Newton : public P_SysOp {
 public:
  P_Newton(const P_List<P_Ctr>& sys, REAL ceil=ibex::Newton::default_ceil, 
	   REAL prec=ibex::Newton::default_prec, REAL ratio_gauss_seidel=ibex::Newton::default_ratio_gauss_seidel) : P_SysOp(sys),
    ceil(ceil), prec(prec), ratio_gauss_seidel(ratio_gauss_seidel) { }
  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); }
  REAL ceil, prec, ratio_gauss_seidel;
};

/* Propagation operator */
class P_Propag : public P_CompOp {
 public:
  P_Propag(const P_List<P_Ctc>& list, REAL ratio=ibex::Propagation::default_ratio, bool incremental=false) : P_CompOp(list), 
    ratio(ratio), incremental(incremental) { }
  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); }
  REAL ratio;
  bool incremental;

};

/* Box narrowing operator */
class P_HC4Revise : public P_CtrOp {
 public:
  P_HC4Revise(const P_Ctr& ctr) : P_CtrOp(ctr),
    var_ratio(var_ratio), w_newton(w_newton) { }
  void accept(P_CtcVisitor& v) const { v.visit(*this); }
  REAL var_ratio, w_newton;
};

/* Box narrowing operator */
class P_BoxNarrow : public P_CtrOp {
 public:
  //  P_BoxNarrow(const P_Equality& equ, REAL var_ratio=ibex::BoxNarrow::default_var_ratio, REAL w_newton=ibex::UnivNewton::default_precision) : P_CtrOp(equ),
  P_BoxNarrow(const P_Ctr& equ, REAL var_ratio=ibex::BoxNarrow::default_var_ratio, REAL w_newton=ibex::UnivNewton::default_precision) : P_CtrOp(equ),
    var_ratio(var_ratio), w_newton(w_newton) { }
  void accept(P_CtcVisitor& v) const { v.visit(*this); }
  REAL var_ratio, w_newton;
};

class P_Precision : public P_Ctc {
 public: 
  P_Precision(REAL prec) : prec(prec) { }

  void accept(P_CtcVisitor& v) const { v.visit(*this); }

  const REAL prec;
};

/* Logical and/or between contractors */
class P_LogicOp : public P_CompOp {
 public:
  P_LogicOp(const P_List<P_Ctc>& vec, bool logic_op) : P_CompOp(vec), logic_op(logic_op) { }

  P_LogicOp(const P_Ctc& ctc1, const P_Ctc& ctc2, bool logic_op) : P_CompOp(ctc1,ctc2), logic_op(logic_op) { }

  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); } 

  bool logic_op;
};

/* And/or loop between iterator-dependant constraints */
class P_LogicOpLoop : public P_LogicOp {
 public:

  P_LogicOpLoop(const char* iter, int first_value, int last_value, const P_List<P_Ctc>& iter_ctc, bool logic_op) : 
    P_LogicOp(iter_ctc, logic_op), iter(strdup(iter)), first_value(first_value), last_value(last_value) { }

  ~P_LogicOpLoop() {
    free((char*) iter);
  }
  
  virtual void accept(P_CtcVisitor& v) const { v.visit(*this); } 

  const char* iter;
  int first_value;
  int last_value;
};

/* Q-Intersection */
class P_QInter : public P_Ctc {
 public:
  P_QInter(const P_ExtendedSymbol& x, const P_ExtendedSymbol& y, int q) : x(x), y(y), q(q) { }

  ~P_QInter() { 
    delete &x; 
    delete &y; 
  }

  void accept(P_CtcVisitor& v) const { v.visit(*this); }

  const P_ExtendedSymbol& x;
  const P_ExtendedSymbol& y;
  int q;
};

/* Load data */
class P_Load : public P_Ctc {
 public:
  P_Load(const char* filename, const P_ExtendedSymbol& x) : 
    filename(filename), x(x) { }

  ~P_Load() { 
    free((char*) filename);
    delete &x; 
  }

  void accept(P_CtcVisitor& v) const { v.visit(*this); }

  const char* filename;
  const P_ExtendedSymbol& x;
};


} // end namespace

} // end namespace

#endif
