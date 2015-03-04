//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap_2.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Sept 11, 2014
// Last Update : Sept 11, 2014
//============================================================================

#include "ibex_CellCostFunc.h"
#include "ibex_OptimData.h"

namespace ibex {


// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapVarLB::CellHeapVarLB(int ind_var) : goal_var(ind_var) { };

double CellHeapVarLB::cost(const Cell& c) const { return c.box[goal_var].lb(); }


// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapVarUB::CellHeapVarUB(int ind_var) : goal_var(ind_var)  { };

double CellHeapVarUB::cost(const Cell& c) const { return c.box[goal_var].ub(); }

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapC3::CellHeapC3(double lb) : loup(lb) { };

double CellHeapC3::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return -((loup - data->pf.lb()) / data->pf.diam() );
	} else {
		ibex_error("CellHeapC3::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapC5::CellHeapC5(double lb) : loup(lb) { };

double CellHeapC5::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return (-(data->pu * (loup - data->pf.lb()) / data->pf.diam()));
	} else {
		ibex_error("CellHeapC5::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapC7::CellHeapC7(int ind_var, double lb) : loup(lb), goal_var(ind_var) {};

double CellHeapC7::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return c.box[goal_var].lb()/(data->pu*(loup-data->pf.lb())/data->pf.diam());
	} else {
		ibex_error("CellHeapC7::cost : invalid cost"); return POS_INFINITY;
	}
}

double CellHeapPU::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  -data->pu;
	} else {
		ibex_error("CellHeapPU::cost : invalid cost"); return POS_INFINITY;
	}
}

double CellHeapPFlb::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  data->pf.lb();
	} else {
		ibex_error("CellHeapPFlb::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

double CellHeapPFub::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  data->pf.ub();
	} else {
		ibex_error("CellHeapPFub::cost : invalid cost"); return POS_INFINITY;
	}
}

}  // end namespace ibex
