//============================================================================
//                                  I B E X                                   
// File        : ibex_SepFixPoint.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_FIXPOINT_H__
#define __IBEX_SEP_FIXPOINT_H__


#include <ibex_IntervalVector.h>
#include <ibex_Sep.h>
#include <ibex_LargestFirst.h>
#include <vector>

// ompute the fixed point of a separator

namespace ibex {
class SepFixPoint : public Sep {

public:
    /**
     * \brief build a fix point.
     *
     * When the Hausdorff distance between
     * two iterations is less than ratio*diameter
     * the fix-point is considered to be reached.
     */
    SepFixPoint(Sep& sep, double ratio=default_ratio);

    /**
     * \brief Delete *this.
     */
    ~SepFixPoint();

    /**
     * \brief Separate a box.
     */
    void separate(IntervalVector &x_in, IntervalVector &x_out);
    
    bool reconstrut(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_old);
    void clearFlags();
    bool setCoutFlags(IntervalVector &x_out, IntervalVector &x0);
    bool setCinFlags(IntervalVector &x_in, IntervalVector &x0);
    
    protected:

    /**
     * \brief The Contractor.
     */
    Sep& sep;

    /**
      * \brief the outer contractor / inner contractor has an impact
      */
    bool impact_cin, impact_cout;

    /**
      * \brief store the firsr box contractor by the inner/ outer contractor
      */
    IntervalVector *first_cin_boxes, *first_cout_boxes;
    int n_in, n_out;


    /**
     * Ratio. When the Hausdorff distance between
     * two iterations is less than ratio*diameter
     * the fix-point is considered to be reached.
     */
    double ratio;

    /** Default ratio used, set to 0.1. */
    static const double default_ratio;

private:
    void fixpoint(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &y);
};

} // namespace ibex

#endif // __IBEX_SEP_FIXPOINT_H__

