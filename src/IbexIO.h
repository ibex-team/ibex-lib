/*---------------------------------------------------------------------------------
 * 
 * Input-output
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

#ifndef __IBEX_IO_H
#define __IBEX_IO_H

#include <fstream>
#include "IbexExpr.h"
#include "IbexSpace.h"
#include "IbexPaving.h"
#include "IbexContractor.h"

namespace ibex {

/* deprecated... (replaced by Loader) */
void read_file(const char* filename, Space& space, const ExtendedSymbol& symbol);

/** \ingroup ctcgroup
 *
 * \brief Data Loader
 *
 * Set domains of a vector of entities by reading a (plain text) file.
 *
 * \author Gilles Chabert
 * \date August 2010
 */
class Loader : public Contractor {
 public:
  /**
   * \param space - 
   * \param filename - the input file. Each row must contain 2*n real numbers, where n is the size
   * of the last dimension of x (see #ibex::Dim::size3()). E.g., if x is a matrix of 1000 rows and 3 columns,
   * then the file must contain 1000 lines, each line containing 6 numbers:<br>
   *  lb(x[i][1])  up(x[i][1]) ... lb(x[i][3])  up(x[i][3])
   * \param x - the vector symbol, with either 1 or 2 dimensions.
   */
  Loader(Space& space, const char* filename, const ExtendedSymbol& x) : Operator(space), filename(strdup(filename)), x(x) { }

  Loader(const Loader& l) : Operator(l.space), Contractor(l), filename(strdup(l.filename)), x(l.x) { }

  Loader* copy() const { return new Loader(*this); }
  
  void contract();

 private:
  const char* filename;
  const ExtendedSymbol& x;
};


void print_var(const Space& space, const ExtendedSymbol&);

/* void plot2D(const char* filename, const Space& space, const ExtendedSymbol& x, const ExtendedSymbol& y, int color); */

/* class ScilabPlot2D { */
/*  public: */
/*    ScilabPlot2D(const char*); */
/*    ~ScilabPlot2D(); */
/*    void write_header(); */
/*    void update_frame(const INTERVAL& dx, const INTERVAL& dy); */
/*    void set_frame(); */
/*    void draw_box(const INTERVAL& x, const INTERVAL& y, int color); */
/*  private: */
/*   fstream fin; */
/*   REAL xmin,xmax,ymin,ymax; */
/*  }; */

/* class Paving2Scilab : public PavingVisitor, ScilabPlot2D { */
/*  public: */
/*   Paving2Scilab(const char* filename, const PavingNode&, int var1, int var2, int color_codes[]); */
/*  private: */
/*   virtual void visit(const PavingNode&); */
/*   virtual void visit(const ContractorNode&); */
/*   virtual void visit(const BisectorNode&); */
/*   int var1; */
/*   int var2; */
/*   int* color_codes;   */
/*   bool no_frame; */
/* }; */

struct RGB {
  public:
/*   RGB() : red(0), green(0), blue(0) { } */
/*   RGB(double red, double green, double blue) : red(red), green(green), blue(blue) { } */
  double red, green, blue;  
};

struct BoxColor {
  RGB color_in;  // color for the interior 
  RGB color_out;  // color for the border 
};


void plot2D(fstream& fin, const Space& space, const ExtendedSymbol& x, const ExtendedSymbol& y, const BoxColor& color);

class PavingPlot : public PavingVisitor {
 public:  
  PavingPlot(fstream& fin, const PavingNode&, int ctc, int var1, int var2, const BoxColor& color);

  PavingPlot(fstream& fin, const PavingNode&, int var1, int var2, const vector<BoxColor>& colors);
 private:
  virtual void visit(const PavingNode&);
  virtual void visit(const ContractorNode&);
  virtual void visit(const BisectorNode&);

  fstream& fin;

  int var1;
  int var2;
  int ctc;
  bool compute_size;
  int size;
};

} // end namespace

#endif
