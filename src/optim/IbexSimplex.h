
#ifndef _IBEX_SIMPLEX_H
#define _IBEX_SIMPLEX_H

#include "IbexSpace.h"
#include "IbexSystem.h"
#include "IbexEvaluator.h"
#include "IbexPropagation.h"
#include "IbexContractor.h"

namespace ibex {
  
 //The system is relaxed by using the Taylor extension.
//Then the simplex algorithm is applied to obtain a new lower bound of [y]
//If simplex does not find any solution, the method returns false, otherwise true
  bool simplex_lower_bounding(const System& sys,  const Evaluator& goal, REAL& loup);

  //Araya+trombe
  bool simplex_lower_bounding_at(const System& sys, const Evaluator& goal, REAL& loup);

//The system is relaxed by using the Taylor extension.
//Then the simplex algorithm is applied to obtain a new uplo
//If a new uplo is found the method returns true  
  bool simplex_update_loup(const System& sys, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point);

  bool simplex_upper_bounding_at(const System& sys, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point);

}

#endif
