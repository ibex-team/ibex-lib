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

#ifndef _IBEX_OPERATOR_PRINTER_H
#define _IBEX_OPERATOR_PRINTER_H

#include <iostream>

namespace ibex {

class Operator;

class Contractor;
class ContractorList;
class Sequence;
class Newton;
class Equation;
//class UnivNewton;
class BoxNarrow;
class Slicer;
//class System;
class Propagation;
class HC4;
class HC4Revise;
//class Box;
class Shaving;
class _3BCID;
class Precision;
class Certification;


class Bisector;
class RoundRobin;
class LargestFirst;
class SmearMax;
 class SmearSum;
class BlockRoundRobin;
class Column;
class Transpose;

class Constraint;
class AtMostNVector;
class Equality;
class Inequality;
class AtMostNVector;
class NotIn;

class OperatorPrinter {
 public:
  OperatorPrinter(std::ostream& os) : os(os), shift(0) { }
  
  void print(const Operator&) const;
  void print(const ContractorList&,int) const;
  void print(const Contractor&) const;
  void print(const Sequence&) const;
  void print(const Newton&) const;
  //void print(const UnivNewton&) const;
  void print(const BoxNarrow&) const;
  void print(const Slicer&) const;
  void print(const Propagation&) const;
  void print(const HC4Revise&) const;
  void print(const HC4&) const;
  void print(const Shaving&) const;
  void print(const _3BCID&) const;
  void print(const Precision&) const;
  void print(const Certification&) const;
//  void print(const GoldsztejnSwapTest&) const;

  void print(const RoundRobin&) const;
  void print(const LargestFirst&) const;
  void print(const SmearMax&) const;
  void print(const SmearSum&) const;
  void print(const BlockRoundRobin&) const;

  void print(const Constraint&) const;
  void print(const Equality&) const;
  void print(const Inequality&) const;
  void print(const AtMostNVector&) const;
  void print(const NotIn&) const;

  mutable std::ostream& os;

 private:
  mutable int shift;
};

} // end namespace

#endif
