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

#include <stdio.h>
#include "IbexIO.h"
#include "IbexEnv.h"
#include "IbexSpace.h"

#define MAX_LINE_SIZE 2000

namespace ibex {

char* fgets2(FILE* f,char* line)   //Afin d'ignorer les lignes qui commencent par %
{ char* r;
  do   { r=fgets(line,256,f);}  while ((line[0]=='%')&&(r!= NULL));
  return r;
}

void read_file(const char* filename, Space& space, const ExtendedSymbol& symbol) {
  int key = symbol.key;
  Dim dim = symbol.dim;
  
  if (dim.dim1>0) throw NonRecoverableException("Cannot initialize 3D-vectors from a file");

  FILE *f;
  REAL lb,ub;

  if ((f = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);
  cerr << "reading file " << filename << endl;
  int i=0;

  int code;
  do {
    code=fscanf(f,"%lf",&lb);
    if (code!=EOF) {
      if (fscanf(f,"%lf",&ub)==EOF) throw NonRecoverableException("Bad file format"); 
      space.domain(key + (i++)) = INTERVAL(lb, ub);
    }
  } while (code!=EOF && i<dim.size());

  if (i<dim.size()) throw NonRecoverableException("Unexpected end of file");
  fclose(f);
}


void Loader::contract() {
  read_file(filename, space, x);  
}

void print_var(const Space& space, const ExtendedSymbol& symbol) {
  int key = symbol.key;
  Dim dim = symbol.dim;
  
  for (int i=0; i<dim.size1(); i++) {
    for (int j=0; j<dim.size2(); j++) {
      for (int k=0; k<dim.size3(); k++) {
	cout << space.domain(key++) << " ";
	if (dim.dim2==0) cout << endl;
      }
      if (j<dim.dim2) cout << endl;
    } 
    if (i<dim.dim1) cout << endl << endl;
  }
}


// void plot2D(const char* filename, const Space& space, const ExtendedSymbol& x, const ExtendedSymbol& y, int color) {

//   ScilabPlot2D plot(filename);

//   if (x.dim.dim2>0 || y.dim.dim2>0)
//     throw NonRecoverableException("Scilab exportation of a paving can only take in argument two 1-dimensional vectors (x and y)");

//   if (x.dim.dim3 != y.dim.dim3)
//     throw NonRecoverableException("Scilab exportation of a paving requires two vectors of the same size");

//   plot.write_header();

//   for (int i=0; i<x.dim.size3(); i++) {
//     plot.update_frame(space.domain(x.key+i), space.domain(x.key+i));
//   }
  
//   plot.set_frame();

//   for (int i=0; i<x.dim.size3(); i++) {
//     plot.draw_box(space.domain(x.key+i), space.domain(y.key+i), color);   
//   }
// }


/*----------------------------------------------------------------------------------------------------*/

// ScilabPlot2D::ScilabPlot2D(const char* filename) :  xmin(BiasPosInf), ymin(BiasPosInf), xmax(BiasNegInf), ymax(BiasNegInf) {
//   fin.open(filename,fstream::out);
// }

// ScilabPlot2D::~ScilabPlot2D() {
//   fin.close();
// }

// void ScilabPlot2D::write_header() {
//   fin << "function  b_draw(X,color)" << endl;
//   fin << "xset(\"pattern\",color);" << endl;
//   fin << "dx=X(:,2)-X(:,1);" << endl;
//   fin << "xfrect(X(1,1),X(2,2),dx(1),dx(2));" << endl;
//   fin << "xset(\"pattern\",0);" << endl;
//   fin << "xrect(X(1,1),X(2,2),dx(1),dx(2));" << endl;
//   fin << "endfunction" << endl;
//   fin << endl;
//   fin << "xmin=-10;ymin=-10;xmax=10;ymax=10;" << endl;
// }

// void ScilabPlot2D::update_frame(const INTERVAL& dx, const INTERVAL& dy) {
//   if (Inf(dx) < xmin) xmin = Inf(dx);
//   if (Sup(dx) > xmax) xmax = Sup(dx);
//   if (Inf(dy) < ymin) ymin = Inf(dy);
//   if (Sup(dy) > ymax) ymax = Sup(dy);
// }

// void ScilabPlot2D::set_frame() {
//   fin << "xsetech([0 0 1 1],[" << xmin << " " << ymin << " " << xmax << " " << ymax << "]);" << endl;
// }

// void ScilabPlot2D::draw_box(const INTERVAL& x, const INTERVAL& y, int color) {
//   fin << "X=[" << Inf(x) << "," << Sup(x) << ";" << Inf(y) << "," << Sup(y) << "]; ";
//   fin << "b_draw(X," << color << ");" << endl;
// }


// Paving2Scilab::Paving2Scilab(const char* filename, const PavingNode& node, int var1, int var2, int color_codes[]) : 
//   ScilabPlot2D(filename), var1(var1), var2(var2), color_codes(color_codes) {
//   write_header();
//   no_frame=true;
//   visit(node);
//   set_frame();
//   no_frame=false;
//   visit(node);
// }

// void Paving2Scilab::visit(const PavingNode& n) {
//   if (no_frame) {
//     update_frame(n.box(var1+1), n.box(var2+1));
//   }
//   n.acceptVisitor(*this);
// }

// void Paving2Scilab::visit(const ContractorNode& c) {
//   if (!no_frame)
//     draw_box(c.box(var1+1), c.box(var2+1), color_codes[c.ctc_number()]);
//   if (!c.leaf()) visit(c.rejected());
// }

// void Paving2Scilab::visit(const BisectorNode& b) {
// //   if (b.left) 
//     visit(b.left());
// //   if (right) 
//     visit(b.right());
// }

/*================================================================================*/

PavingPlot::PavingPlot(fstream& fin, const PavingNode& node, int ctc, int var1, int var2, const BoxColor& color) : fin(fin), var1(var1), var2(var2), ctc(ctc), size(0) {
  compute_size=true;
  visit(node);
  compute_size=false;
  fin << 1 << " " << size << endl;
  fin << color.color_in.red << " " << color.color_in.green << " " << color.color_in.blue << " ";
  fin << color.color_out.red << " " << color.color_out.green << " " << color.color_out.blue << endl;
  visit(node);
}

PavingPlot::PavingPlot(fstream& fin, const PavingNode& node, int var1, int var2, const vector<BoxColor>& colors) : fin(fin), var1(var1), var2(var2), ctc(-1), size(0) {

  compute_size=true;
  visit(node);
  compute_size=false;
  fin << colors.size() << " " << size << endl;
  for (vector<BoxColor>::const_iterator it=colors.begin(); it!=colors.end(); it++) {
    fin << it->color_in.red << " " << it->color_in.green << " " << it->color_in.blue << " ";
    fin << it->color_out.red << " " << it->color_out.green << " " << it->color_out.blue << endl;
  }
  visit(node);
}

void PavingPlot::visit(const PavingNode& n) {
  n.acceptVisitor(*this);
}

void PavingPlot::visit(const ContractorNode& c) {
//  cout << "ctc=" << ctc << " " << c.ctc_number() << endl;
  if (ctc==-1 || c.ctc_number() == ctc) { 
    if (compute_size) 
      size++; 
    else {
      fin << Inf(c.box(var1+1)) << " " << Sup(c.box(var1+1)) << " ";
      fin << Inf(c.box(var2+1)) << " " << Sup(c.box(var2+1)) << " ";
      fin << (ctc==-1? c.ctc_number() : 0) << endl;
    }
  }
  if (!c.leaf()) visit(c.rejected());
}

void PavingPlot::visit(const BisectorNode& b) {
  visit(b.left());
  visit(b.right());
}

/*================================================================================*/

void plot2D(fstream& fin, const Space& space, const ExtendedSymbol& x, const ExtendedSymbol& y, const BoxColor& color) {

  if (x.dim.dim2>0 || y.dim.dim2>0)
    throw NonRecoverableException("plot can only take in argument two 1-dimensional vectors (x and y)");

  if (x.dim.dim3 != y.dim.dim3)
    throw NonRecoverableException("plot requires two vectors of the same size");

  fin << 1 << " " << x.dim.size3() << endl;
  fin << color.color_in.red << " " << color.color_in.green << " " << color.color_in.blue << " ";
  fin << color.color_out.red << " " << color.color_out.green << " " << color.color_out.blue << endl;

  for (int i=0; i<x.dim.size3(); i++) {
    const INTERVAL& dx=space.domain(x.key+i);
    const INTERVAL& dy=space.domain(y.key+i);
    fin << Inf(dx) << " " << Sup(dx) << " " << Inf(dy) << " " << Sup(dy) << " 0 " << endl;
  }
}

} // end namespace
