//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderLineSearch.h
// Author      : Antoine Marendet
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 12, 2018
// Last Update : Nov 12, 2018
//============================================================================

#include "ibex_LoupFinder.h"
#include "ibex_Array.h"

namespace ibex {

class LoupFinderCompo : public LoupFinder {
public:
    LoupFinderCompo(const Array<LoupFinder>& loup_finders);
    virtual ~LoupFinderCompo();

    void add_property(const IntervalVector& init_box, BoxProperties& prop);
    std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);

private:
    Array<LoupFinder> loup_finders_;
};
}