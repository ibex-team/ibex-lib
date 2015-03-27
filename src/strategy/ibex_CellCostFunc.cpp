//============================================================================
//                                  I B E X                                   
// File        : ibex_CellCost_2.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Sept 11, 2014
// Last Update : Sept 11, 2014
//============================================================================

#include "ibex_CellCostFunc.h"
#include "ibex_OptimData.h"

namespace ibex {

CellCostFunc::CellCostFunc(bool depends_on_loup) : depends_on_loup(depends_on_loup) {

}

CellCostFunc* CellCostFunc::get_cost(criterion crit, int goal_var) {
	switch (crit) {
	case LB :    return new CellCostVarLB(goal_var); break;
	case UB :    return new CellCostVarUB(goal_var); break;
	case C3 :    return new CellCostC3();            break;
	case C5 :    return new CellCostC5();            break;
	case C7 :    return new CellCostC7(goal_var);    break;
	case PU :    return new CellCostPU();            break;
	case PF_LB : return new CellCostPFlb();          break;
	case PF_UB : return new CellCostPFub();          break;
	default:     ibex_error("CellCostFunc::get_cost : error  wrong criterion.");
	             return NULL;
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------

CellCostVarLB::CellCostVarLB(int ind_var) : CellCostFunc(false), goal_var(ind_var) {

}

double CellCostVarLB::cost(const Cell& c) const {
	return c.box[goal_var].lb();
}


// -----------------------------------------------------------------------------------------------------------------------------------

CellCostVarUB::CellCostVarUB(int ind_var) : CellCostFunc(false), goal_var(ind_var) {

}

double CellCostVarUB::cost(const Cell& c) const {
	return c.box[goal_var].ub();
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostC3::CellCostC3(double lb) : CellCostFunc(true), loup(lb) {

}

double CellCostC3::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return -((loup - data->pf.lb()) / data->pf.diam() );
	} else {
		ibex_error("CellCostC3::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostC5::CellCostC5(double lb) : CellCostFunc(true), loup(lb) {

}

double CellCostC5::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return (-(data->pu * (loup - data->pf.lb()) / data->pf.diam()));
	} else {
		ibex_error("CellCostC5::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostC7::CellCostC7(int ind_var, double lb) : CellCostFunc(true), loup(lb), goal_var(ind_var) {

}

double CellCostC7::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return c.box[goal_var].lb()/(data->pu*(loup-data->pf.lb())/data->pf.diam());
	} else {
		ibex_error("CellCostC7::cost : invalid cost"); return POS_INFINITY;
	}
}

CellCostPU::CellCostPU() :  CellCostFunc(false) {

}

double CellCostPU::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  -data->pu;
	} else {
		ibex_error("CellCostPU::cost : invalid cost"); return POS_INFINITY;
	}
}


CellCostPFlb::CellCostPFlb() :  CellCostFunc(false) {

}
double CellCostPFlb::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  data->pf.lb();
	} else {
		ibex_error("CellCostPFlb::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostPFub::CellCostPFub() :  CellCostFunc(false) {

}

double CellCostPFub::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  data->pf.ub();
	} else {
		ibex_error("CellCostPFub::cost : invalid cost"); return POS_INFINITY;
	}
}

}  // end namespace ibex
