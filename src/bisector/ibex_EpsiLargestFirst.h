//============================================================================
//                                  I B E X                                   
// File        : Relative Largest First bisector
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : July 19, 2012
// Last Update : July 19, 2012
//============================================================================

#ifndef __IBEX_REL_LARGEST_FIRST_H__
#define __IBEX_REL_LARGEST_FIRST_H__

#include "ibex_Bsc.h"
#include "ibex_Backtrackable.h"

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief Relative-largest-first bisector.
 *
 */
class EpsiLargestFirst : public Bsc {
 public:
  /**
   * \brief Create a bisector with relative largest first heuristic.
   *
   * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
   *            bisected interval. Default value is 0.45.
   */
    EpsiLargestFirst( const Vector &init, double ratio=Bsc::default_ratio);

 

  /**
   * \brief Bisect the largest box.
   *
   * called by Bsc::bisect 
   */
  virtual std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box);



  /**
   * \brief Ratio to chose the split point.
   *
   * Ratio between the diameters of the left and right parts of a bisected
   * interval.
   */
  const double ratio;

  /**
   * \brief Initial diameter of each variable to compute the relative diameter
   */
  Vector init_epsi;

 
  };

} // end namespace ibex

#endif // __IBEX_LARGEST_FIRST_H__
