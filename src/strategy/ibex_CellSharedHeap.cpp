//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap_2.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Sept 11, 2014
// Last Update : Sept 11, 2014
//============================================================================

#include "ibex_CellSharedHeap.h"


namespace ibex {


// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapVarLB::CellHeapVarLB(int ind_var, int ind_crit) : CellHeap(ind_crit, false), goal_var(ind_var) { };

double CellHeapVarLB::cost(const Cell& c) const { return c.box[goal_var].lb(); }

CellHeapVarLB* CellHeapVarLB::init_copy() const {
	return new CellHeapVarLB(goal_var, get_id());
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapVarUB::CellHeapVarUB(int ind_var, int ind_crit) : CellHeap(ind_crit,false), goal_var(ind_var)  { };

double CellHeapVarUB::cost(const Cell& c) const { return c.box[goal_var].ub(); }

CellHeapVarUB* CellHeapVarUB::init_copy() const {
	return new CellHeapVarUB(goal_var, get_id());
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapC3::CellHeapC3( int ind_crit, double lb) : CellHeap(ind_crit, true), loup(lb) { };

double CellHeapC3::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return -((loup - data->pf.lb()) / data->pf.diam() );
	} else {
		ibex_error("CellHeapC3::cost : invalid cost"); return POS_INFINITY;
	}
}

CellHeapC3* CellHeapC3::init_copy() const {
	return new CellHeapC3(get_id(), loup);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapC5::CellHeapC5( int ind_crit, double lb) : CellHeap(ind_crit, true), loup(lb) { };

double CellHeapC5::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return (-(data->pu * (loup - data->pf.lb()) / data->pf.diam()));
	} else {
		ibex_error("CellHeapC5::cost : invalid cost"); return POS_INFINITY;
	}
}

CellHeapC5* CellHeapC5::init_copy() const {
	return new CellHeapC5(get_id(),loup);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapC7::CellHeapC7(int ind_var,  int ind_crit, double lb) : CellHeap(ind_crit, true), loup(lb), goal_var(ind_var) {};

double CellHeapC7::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return c.box[goal_var].lb()/(data->pu*(loup-data->pf.lb())/data->pf.diam());
	} else {
		ibex_error("CellHeapC7::cost : invalid cost"); return POS_INFINITY;
	}
}

CellHeapC7* CellHeapC7::init_copy() const {
	return  new CellHeapC7(goal_var,get_id(), loup);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapPU::CellHeapPU(int ind_crit) : CellHeap(ind_crit, false) { };

double CellHeapPU::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  -data->pu;
	} else {
		ibex_error("CellHeapPU::cost : invalid cost"); return POS_INFINITY;
	}
}

CellHeapPU* CellHeapPU::init_copy() const {	return new CellHeapPU( get_id());}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapPFlb::CellHeapPFlb(int ind_crit) : CellHeap(ind_crit, false) { };

double CellHeapPFlb::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  data->pf.lb();
	} else {
		ibex_error("CellHeapPFlb::cost : invalid cost"); return POS_INFINITY;
	}
}

CellHeapPFlb* CellHeapPFlb::init_copy() const {	return new CellHeapPFlb( get_id());}

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapPFub::CellHeapPFub(int ind_crit) : CellHeap(ind_crit, false) { };

double CellHeapPFub::cost(const Cell& c) const {
	const OptimData *data = &(c.get<OptimData>());
	if (data) {
		return  data->pf.ub();
	} else {
		ibex_error("CellHeapPFub::cost : invalid cost"); return POS_INFINITY;
	}
}

CellHeapPFub* CellHeapPFub::init_copy() const {	return new CellHeapPFub( get_id());}

}  // end namespace ibex
