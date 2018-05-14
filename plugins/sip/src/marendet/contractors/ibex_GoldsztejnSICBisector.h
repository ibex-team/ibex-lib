//============================================================================
//                                  I B E X                                   
// File        : ibex_GoldsztejnSICBisector.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_GOLDSZTEJNSICBISECTOR_H__
#define __SIP_IBEX_GOLDSZTEJNSICBISECTOR_H__

#include "system/ibex_SIPSystem.h"

#include "ibex_CellCtc.h"

namespace ibex {
/**
 * \brief GoldsztejnSICBisector
 *
 * Bisect parameter boxes if the evaluation of the bisection of a parameter box if the ratio of rel_diam() of the original box
 * over the bisected box is less than a certain value.
 */
class GoldsztejnSICBisector : public CellCtc {
    const SIPSystem& system_;
    static constexpr double default_ratio = 0.8;
    const double ratio_;
public:
    GoldsztejnSICBisector(const SIPSystem& system, double ratio=default_ratio);
    ~GoldsztejnSICBisector();
    void contractCell(Cell& cell);
};

} // end namespace ibex

#endif // __SIP_IBEX_GOLDSZTEJNSICBISECTOR_H__
