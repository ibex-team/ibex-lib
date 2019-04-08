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
Function f("x","x");
//! [prop-ctc-1]

// A contractor
class MyCtc : public Ctc {
public:
  void contract(IntervalVector& box) {
    Interval y = f.eval(box); // this line is assumed to be expensive
    // ...
  }
};
//! [prop-ctc-1]

//! [prop-class-name]
class BxpImage : public Bxp {
public:
  // to be completed...

  Interval image;
};
//! [prop-class-name]
}

namespace prop2 {

Function f("x","x");

//! [prop-id]
class BxpImage : public Bxp {
public:
  BxpImage(const IntervalVector& box) :
    Bxp(id), image(f.eval(box)) { }

  // to be completed...

  Interval image;
  static const long id;
};

const long BxpImage::id = next_id();
//! [prop-id]
}


namespace prop3 {

Function f("x","x");

class BxpImage : public Bxp {
public:
  BxpImage(const IntervalVector& box) : Bxp(id), image(f.eval(box)) { }

  //! [prop-update]
  void update(const BoxEvent& event, const BoxProperties& prop) {
    image = f.eval(event.box);
  }
  //! [prop-update]

  //! [prop-copy]
  Bxp* copy(const IntervalVector& box, const BoxProperties& prop) const {
    return new BxpImage(*this); // implicit copy constructor is fine
  }
  //! [prop-copy]

  Interval image;
  static const long id;
};

const long BxpImage::id = next_id();

//! [prop-ctc-2]

class MyCtc : public Ctc {
public:

  /* Add the required property inside the map.
   * (This function is automatically called by the search). */
  void add_property(IntervalVector& box, BoxProperties& prop) {
    // if the property is not already in the list
    // (which is possible since another operator requiring it may
    // have already added it)
    if (!prop[BxpImage::id])
      // create an initial property value, and add it
      prop.add(new BxpImage(box));
  }

  /* Contract a box with associated properties. */
  void contract(IntervalVector& box, ContractContext& context) {
    // Get the desired property from the map, by its id
    // (a cast is necessary because all properties are typed Bxp*)
    BxpImage* bxp=(BxpImage*) context.prop[BxpImage::id];

    if (bxp==NULL) {
      // This happens if the property is not present.
      // It is much more safe to handle this case
      // ...
    } else {
      // Obtain directly the image (without recalculating it)
      Interval y=bxp->image;
      // .....
    }
  }
};
//! [prop-ctc-2]
}

namespace prop4 {
Function f("x","x");

//! [prop-lazy-update]
class BxpImage : public Bxp {
public:
  // The class contains 2 new fields:
  // box: a reference to the box needs to be stored
  // to perform the evaluation at any time.
  // up2date: memorize whether the image is up to date.
  BxpImage(const IntervalVector& box) : Bxp(id), box(box), up2date(false) { }

  void update(const BoxEvent& event, const BoxProperties& prop) {
    // do nothing for the moment!
    up2date=false;
  }

  Bxp* copy(const IntervalVector& box, const BoxProperties& prop) const {
    return new BxpImage(box,image,up2date);
  }

  // Return the image of f.
  const Interval& get() {
    if (!up2date) {
      image = f.eval(box);
      up2date=true;
    }
    return image;
  }

  static const long id;

protected:
  BxpImage(const IntervalVector& box, const Interval& image, bool up2date) :
    Bxp(id), box(box), image(image), up2date(up2date) { }

  const IntervalVector& box;
  Interval image;
  bool up2date;
};
//! [prop-lazy-update]

const long BxpImage::id = next_id();

//! [prop-dependencies]
class BxpImageWidth : public Bxp {
public:
  BxpImageWidth(const IntervalVector& box) : Bxp(id), w(box.max_diam()) {
  // dependencies is a field inherited from Bxp
    dependencies.push_back(BxpImage::id);
  }

  void update(const BoxEvent& event, const BoxProperties& prop) {
    BxpImage* bxp=(BxpImage*) prop[BxpImage::id];

    if (bxp==NULL) {
      // This happens if the property is not present.
      // It is much more safe to handle this case
      // ...
    } else {
     // note: we lose laziness here
      w=bxp->get().diam();
    }
  }

  double w; // the width
  static const long id;
};
//! [prop-dependencies]
}

int main() {

  {
    //! [bsc-different-prec]
    double _prec[]={1e-8,1e-8,1e-4,1};

    Vector prec(4,_prec);

    RoundRobin rr(prec);
    //! [bsc-different-prec]
  }

}
