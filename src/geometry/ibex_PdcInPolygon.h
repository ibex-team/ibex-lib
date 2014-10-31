//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcInPolygon.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#ifndef __IBEX_PDC_IN_POLYGON_H__
#define __IBEX_PDC_IN_POLYGON_H__

#include "ibex_Pdc.h"
#include <vector>

namespace ibex {

/**
 * \ingroup geometry
 *
 * \brief Tests if a box is inside a polygon.
 *
 * The polygon is not necessarily convex.
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
 */
class PdcInPolygon : public Pdc {
public:

	/**
	 * \brief Create the predicate with the list of segments passed as argument.
     *
     * A polygon is defined as an union of segments given in a counter-clockwise order.
     * See unit test for an example of usage
     *
     * \param _ax list of x coordinate of the first point of each segment
     * \param _ay list of y coordinate of the first point of each segment
     * \param _bx list of x coordinate of the second point of each segment
     * \param _by list of y coordinate of the second point of each segment
	 */
	PdcInPolygon(std::vector<double>& _ax, std::vector<double>& _ay, std::vector<double>& _bx, std::vector<double>& _by);

	/**
	 * \brief Test the box.
	 */
	virtual BoolInterval test(const IntervalVector& box);

protected:
	/**
     * Inverse the inner and outer part of the polygon.
	 */
    void inv();

    /**
     * Definition of the segment of the polygon
     */
	std::vector<double>& ax;
    std::vector<double>& ay;
    std::vector<double>& bx;
    std::vector<double>& by;

private:
    //    /**
    //     * @brief Check if a point is inside the polygon
    //     * @param x coordinate of the point
    //     * @param y coordinate of the point
    //     * @return true of the point is inside the polygon, false otherwise
    //     */
    //    bool pointInPolygon2(double x, double y);
    void precalc_values();

    int pnpoly(double x, double y);

    std::vector<double> multiple, constant;

    bool inverse;

};

} // namespace ibex

#endif // __IBEX_PDC_IN_POLYGON_H__
