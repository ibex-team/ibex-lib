//============================================================================
//                                  I B E X                                   
// File        : Relative Largest First bisector
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : April 17, 2014
// Last Update : April 17, 2014
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
class RelLargestFirst : public Bsc {
 public:
  /**
   * \brief Create a bisector with relative-largest first heuristic.
   *  The bisector bisects following the variable of indices: max_{i}(box[i].diam()/init[i].diam()).
   *
   * \param init - the initial domain under study to compute the relative diameter.
   * 	The stored diameter of each variable is bounded by 1.e20.
   * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
   *            bisected interval. Default value is 0.45.
   */
    RelLargestFirst( const IntervalVector &init, double ratio=Bsc::default_ratio);

  /**
   * \brief Bisect the relative-largest box.
   * max_{i}(box[i].diam()/init[i].diam()).
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
  Vector init_diam;

 
  };

} // end namespace ibex

#endif // __IBEX_REL_LARGEST_FIRST_H__
