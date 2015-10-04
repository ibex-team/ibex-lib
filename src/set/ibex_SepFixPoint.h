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

// Compute the fixed point of a separator
/**
 * \ingroup Set
 * \brief FixPoint of a separator
 * The fixpoint of a separator is computed by calling the <separate> function
 * on a given box until the Hausdorff distance between two iterations is less than
 * a given ratio. This operation can be seen as a contractor on the boundary of the solution set.
 *
 * At the end, two boxes x_in and x_out, need to be reconstructed
 * by merging the resulting box and parts of the initial box which
 * belong (or not) to the solution set.
 *
 */
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
      * \brief store the first box contracted by the inner / outer contractor
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

    /**
     * Internal function used to compute the fix point of a separator.
     */
    
    /**
     * @brief clearFlags : reset impact_cin, impact_cout flags to false
     * delete first_cin_boxes and first_cout_boxes
     * n_in =0 and n_out = 0 
     */
    void clearFlags();
    /**
     * @brief setCoutFlags
     * if x_out is stritly included in x0 set impact_cout to true
     * and put one box which belongs to x0 \ x_out into first_cout_boxes
     *
     * @param x_out outer box resulting from the separation
     * @param x0 initial box separated.
     */
    void setCoutFlags(IntervalVector &x_out, IntervalVector &x0);

    /**
     * @brief setCinFlags
     * if x_out is stritly included in x0 set impact_cin to true
     * and put one box which belongs to x0 \ x_in into first_cin_boxes
     *
     * @param x_in inner box resulting from the separation
     * @param x0 initial box separated.
     */
    void setCinFlags(IntervalVector &x_in, IntervalVector &x0);

    /**
     * @brief reconstrut 
     * At this end of the separation process we have the box x,
     * stored in x_in and x_out.
     * this function reconstructs x_in and x_out
     * such as 
     *      - x_in \cup x_out = x_old and
     *      - x_in \cap x_out = x
     *
     */
    bool reconstrut(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_old);
    
};

} // namespace ibex

#endif // __IBEX_SEP_FIXPOINT_H__

