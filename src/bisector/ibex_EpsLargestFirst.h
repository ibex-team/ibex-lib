//============================================================================
//                                  I B E X                                   
// File        : Epsilon Largest First bisector
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : April 17, 2014
// Last Update : April 17, 2014
//============================================================================

#ifndef __IBEX_EPSI_LARGEST_FIRST_H__
#define __IBEX_EPSI_LARGEST_FIRST_H__

#include "ibex_Bsc.h"
#include "ibex_Backtrackable.h"

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief Epsilon-largest-first bisector.
 *
 */
class EpsLargestFirst : public Bsc {
 public:
  /**
   * \brief Create a bisector with a heuristic which depends on the required precision on each variable.
   *  The bisector bisects following the variable of indices: max_{i}(box[i].diam()/init[i]).
   *
   * \param init - the initial precision required on each variable.
   * 	The stored precision of each variable is bounded by 1.e20.
   * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
   *            bisected interval. Default value is 0.45.
   */
    EpsLargestFirst( const Vector &init, double ratio=Bsc::default_ratio);

 

  /**
   * \brief Bisect the Epsilon-largest-firsts box.
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
  Vector init_eps;

 
  };

} // end namespace ibex

#endif // __IBEX_EPSI_LARGEST_FIRST_H__
