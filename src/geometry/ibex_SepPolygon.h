//============================================================================
//                                  I B E X
// File        : Separator for the constraint : Point inside a ppolygon
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_SEP_POLYGON_H__
#define __IBEX_SEP_POLYGON_H__

#include "ibex_SepBoundaryCtc.h"
#include "ibex_CtcUnion.h"
#include "ibex_PdcInPolygon.h"

using namespace std;
// TODO  : Remove proprietary code and add option to choose the test
// TODO Write tests.
namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Separator for Point inside a polygon.
 *
 * This separator can be used even if the polygon is not convex
 *
 * The polygon is defined by an union of oriented
 * segment given in counter-clockwise order.
 *
 * Example :
 *  The polygon ABCDE is composed of 5 segments
 *  AB -- BC -- CD -- DE -- EA
 *
 *
 * A------------------------------- E
 *  -                            -
 *   -                          -
 *    -                       -
 *     -          Area       -
 *      -        Inside     - D
 *       -        the        -
 *        -     Polygon       -
 *         -                   -
 *          -                   - C
 *           -            ------
 *            -     ------
 *              ----
 *            B
 *
 * Principle
 *
 * To our knowledge, no explicit contractor for the constraint
 * "Point in inside the polygon" exists when the polygon is non convex. Only a contractor
 * for the border is available.
 * To solve this issue, with an inner an outer set,
 * the separator approach needs to be used.
 * From an initial box, the minimal contractor on the border of the polygon is called
 * and a test is used to classify each removed part into x_in and x_out.
 *
 *
 */
class SepPolygon : public SepBoundaryCtc {

public:

	/**
	 * Create a Separator with the polygon passed as argument.
     * 
     * A polygon is defined as an union of segments given in a counter-clockwise order.
     * See unit test for an example of usage
     *
     * The polygon boundary contractor is composed of a union of
     * contractor on segments (CtcSegment).
     * This contractor is minimal as an union of minimal contractors.
     * See #ibex::SepBoundaryCtc.
     *
     * \param _ax list of x coordinate of the first point of each segment
     * \param _ay list of y coordinate of the first point of each segment
     * \param _bx list of x coordinate of the second point of each segment
     * \param _by list of y coordinate of the second point of each segment
	 */
    SepPolygon(vector<double>& _ax, vector<double>& _ay, vector<double>& _bx, vector<double>& _by);

	/**
	 * \brief Delete this.
	 */
   ~SepPolygon();

};

} // end namespace

#endif // __IBEX_SEP_POLYGON_H__
