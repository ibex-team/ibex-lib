//============================================================================
//                                  I B E X
// File        : doc-strategy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2014
// Last Update : Apr 25, 2014
//============================================================================


#include "ibex.h"

#ifndef _IBEX_WITH_SOLVER_
#error "You need to install the IbexSolve plugin (--with-solver)."
#endif

#ifndef IBEX_BENCHS_DIR
  #define IBEX_BENCHS_DIR "../plugins/solver/benchs"
#endif

using namespace std;
using namespace ibex;

/**
 * This file contains examples of the documentation.
 */

namespace prop1 {
//! [prop-ctc-1]

// A contractor
class MyCtc : public Ctc {
public:
  void contract(IntervalVector& box) {
    if (box.max_diam()>0.1) {
      // do something
    } else {
      // do something else
    }
  }
};
//! [prop-ctc-1]

//! [prop-class-name]
class BxpIsSmall : public Bxp {
public:
  // to be completed...

  bool is_small; // true if width<0.1
};
//! [prop-class-name]
}

namespace prop2 {
//! [prop-id]
class BxpIsSmall : public Bxp {
public:
  BxpIsSmall(const IntervalVector& box) : Bxp(id), is_small(box.max_diam()<0.1) { }

  // to be completed...

  bool is_small;
  static const long id;
};

const long BxpIsSmall::id = next_id();
//! [prop-id]
}


namespace prop3 {
//! [prop-update]
class BxpIsSmall : public Bxp {
public:
  BxpIsSmall(const IntervalVector& box) : Bxp(id), is_small(box.max_diam()<0.1) { }

  void update(const BoxEvent& event, const BoxProperties& prop) {
  // if the event is a contraction and the width is already
  // less than 0.1, we don't recalculate it!
    if (!is_small || event.type!=BoxEvent::CONTRACT) {
      is_small = event.box.max_diam() < 0.1;
    }
  }
  //! [prop-update]

  //! [prop-copy]
  Bxp* update_copy(const IntervalVector& box, const BoxProperties& prop) const {
    return new BxpIsSmall(*this); // implicit copy constructor is fine
  }
  //! [prop-copy]

  bool is_small;
  static const long id;
};
//! [prop-update]

const long BxpIsSmall::id = next_id();

//! [prop-ctc-2]

class MyCtc : public Ctc {
public:

  /* Add the required property inside the map.
   * (This function is automatically called by the search). */
  void add_property(IntervalVector& box, BoxProperties& prop) {
    // if the property is not already in the list
    if (!prop[BxpIsSmall::id])
      // create an initial property value, and add it
      prop.add(new BxpIsSmall(box));
  }

  /* Contract a box with associated properties. */
  void contract(IntervalVector& box, BoxProperties& prop) {
    // Get the desired property from the map, by its id
    // (a cast is necessary because all properties are typed Bxp*)
    BxpIsSmall* bxp=(BxpIsSmall*) prop[BxpIsSmall::id];

    // ... and make directly the right choice without recalculating the width!
    if (bxp->is_small) {
      // do something
    } else {
      // do something else
    }
  }
};
//! [prop-ctc-2]
}

namespace prop4 {
//! [prop-ctc-3]
class MyCtc : public Ctc {
public:
  void contract(IntervalVector& box) {
    if (box.max_diam()>0.1) {
    	if (box.perimeter()>0.1) {
    		// do something
    	} else {
    		// do something else
    	}
    } else {
    	// do still something else
    }
  }
};
//! [prop-ctc-3]
}

namespace prop5 {

using prop3::BxpIsSmall;

//! [prop-update]
class BxpPerimeter : public Bxp {
public:
  BxpPerimeter(const IntervalVector& box) : Bxp(id), is_small(box.max_diam()<0.1) { }

  void update(const BoxEvent& event, const BoxProperties& prop) {
	  BxpIsSmall* bxp=(BxpIsSmall*) prop[BxpIsSmall::id];

	  if (bxp && bxp->is_small)
	  // if the event is a contraction and the width is already
	  // less than 0.1, we don't recalculate it!
	  if (!is_small || event.type!=BoxEvent::CONTRACT) {
		  is_small = event.box.max_diam() < 0.1;
	  }
  }
  //! [prop-update]

  //! [prop-copy]
  Bxp* update_copy(const IntervalVector& box, const BoxProperties& prop) const {
    return new BxpIsSmall(*this); // implicit copy constructor is fine
  }
  //! [prop-copy]

  bool is_small;
  static const long id;
};
//! [prop-update]

const long BxpIsSmall::id = next_id();

int main() {

  {
  //! [bsc-different-prec]
  double _prec[]={1e-8,1e-8,1e-4,1};

  Vector prec(4,_prec);

  RoundRobin rr(prec);
  //! [bsc-different-prec]
  }

}
