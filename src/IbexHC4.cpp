/*---------------------------------------------------------------------------------
 * 
 * Hull Consistency with HC4
 * -------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#include "IbexHC4.h"
#include "IbexHC4Revise.h"
#include "IbexPropagation.h"
#include "IbexOctum.h"

namespace ibex {

HC4::HC4(const CSP& csp, REAL ratio, bool incremental) :
  Operator(csp.space), Propagation(ContractorList(ToHC4Revise(),csp),csp.space, ratio, incremental) {
}

HC4::HC4(const HC4& hc4) : Operator(hc4.space), Propagation(hc4) { }

}
