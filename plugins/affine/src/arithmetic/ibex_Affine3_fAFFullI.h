/* ============================================================================
 * D Y N I B E X - Definition of the Affine3 class based on fAFFull version 1
 * ============================================================================
 * Copyright   : ENSTA ParisTech
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Julien Alexandre dit Sandretto and Alexandre Chapoutot
 * Created     : Jul 18, 2014
 * Sponsored   : This research benefited from the support of the "Chair Complex Systems Engineering - Ecole Polytechnique, THALES, DGA, FX, DASSAULT AVIATION, DCNS Research, ENSTA ParisTech, Telecom ParisTech, Fondation ParisTech and FDO ENSTA"
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_AFFINE3_FAFFULLI_H_
#define IBEX_AFFINE3_FAFFULLI_H_

#include "ibex_Interval.h"

#include <list>

namespace ibex {

template<class T>  class AffineMain;

  class AF_fAFFullI {


    friend class AffineMain<AF_fAFFullI>;
    //    friend std::ostream& operator<<(std::ostream& os, const Affine2Main<AF_fAFFullI>&  x);

    static unsigned long int _counter;

  private:
    /**
     * Code for the particular case:
     * if the affine form is actif, _n>1  and _n is the size of the affine form
     * if the set is degenerate, _n = 0 or itv().diam()< AF_EC()
     * if the set is empty, _n = -1
     * if the set is ]-oo,+oo[, _n = -2
     * if the set is [a, +oo[ , _n = -3 and _err= a
     * if the set is ]-oo, a] , _n = -4 and _err= a
     *
     */
    double _center;
    std::list< std::pair<int, double> > _rays;
    Interval _garbage;

  
  public:
    AF_fAFFullI (double center, std::list<std::pair<int,double> > rays, Interval garbage);

    /** \brief  Delete the affine form */
    virtual ~AF_fAFFullI();
    

  };

  inline AF_fAFFullI::AF_fAFFullI (double center, std::list<std::pair<int,double> > rays, Interval garbage) :
    _center(center), _rays(rays), _garbage(garbage) {

  }

  inline AF_fAFFullI::~AF_fAFFullI() {
    if (!_rays.empty()) { _rays.clear(); }
  }




}



#endif /* IBEX_AFFINE3_FAFFULL_H_ */
