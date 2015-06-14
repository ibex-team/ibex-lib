//============================================================================
//                                  I B E X
// File        : doc-separator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 31, 2015
// Last Update : Jan 31, 2015
//============================================================================


#include "ibex.h"

#include <fstream>

using namespace std;
using namespace ibex;

ofstream output;

/**
 * This file contains examples of the documentation.
 */

int main() {

  output.open ("doc-separator.txt");

  output << "================= this file is generated ==============" << endl;

  {
  output << "! [sep-ctc-pair-O]" << endl;
  //! [sep-ctc-pair-C]

  //! [sep-ctc-pair-C]
  output << "! [sep-ctc-pair-O]" << endl;
  }

  {
  output << "! [sep-fwd-bwd-O]" << endl;
  //! [sep-fwd-bwd-C]

  // Define the function
  Function f("x", "y", "x - y");

  // Build the separator for the set X = {x, y | x - y < 0}
  SepFwdBwd s(f, LT);

  // Note: in a similar way, it is possible to define the separator
  // for the complementary set  :  X = {x, y | x - y >= 0}
  //SepFwdBwd s_compl(f, GEQ);

  double _box[2][2] = {{0,3},{1,2}};
  IntervalVector box(2,_box);
  IntervalVector x_in=box;
  IntervalVector x_out=box;

  s.separate(x_in,x_out);

  output << "result of inner contraction=" << x_in << endl;
  output << "result of outer contraction=" << x_out << endl;

  //! [sep-fwd-bwd-C]
  output << "! [sep-fwd-bwd-O]" << endl;
  }

  {
  output << "! [sep-boundary-O]" << endl;
  //! [sep-boundary-C]

  // Define the function
  Function f("x", "y", "x - y");

  // Build a contractor for the boundary
  CtcFwdBwd c(f,EQ);

  // Build a predicate for the inside
  PdcFwdBwd p(f,LEQ);

  // Build the separator for the set X = {x, y | x - y < 0}
  SepBoundaryCtc s(c,p);

  double _box[2][2] = {{0,3},{1,2}};
  IntervalVector box(2,_box);
  IntervalVector x_in=box;
  IntervalVector x_out=box;

  s.separate(x_in,x_out);

  output << "result of inner contraction=" << x_in << endl;
  output << "result of outer contraction=" << x_out << endl;

  //! [sep-boundary-C]
  output << "! [sep-boundary-O]" << endl;
  }

}
