//============================================================================
//                                  I B E X                                   
// File        : ibex_CellCost_2.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Sept 11, 2014
// Last Update : Sept 11, 2014
//============================================================================

#include "ibex_CellCostFunc.h"
#include "ibex_BxpOptimData.h"

using namespace std;

namespace ibex {

CellCostFunc::CellCostFunc(const ExtendedSystem& sys, bool depends_on_loup) : sys(sys), depends_on_loup(depends_on_loup) {

}

void CellCostFunc::set_loup(double lb) {

}

void CellCostFunc::add_property(BoxProperties& map) {

}

void CellCostFunc::set_optim_data(Cell& c) {

}

CellCostFunc* CellCostFunc::get_cost(const ExtendedSystem& sys, criterion crit, int goal_var) {
	switch (crit) {
	case LB :    return new CellCostVarLB(sys, goal_var); break;
	case UB :    return new CellCostVarUB(sys, goal_var); break;
	case C3 :    return new CellCostC3(sys);              break;
	case C5 :    return new CellCostC5(sys);              break;
	case C7 :    return new CellCostC7(sys, goal_var);    break;
	case PU :    return new CellCostPU(sys);              break;
	case PF_LB : return new CellCostPFlb(sys);            break;
	case PF_UB : return new CellCostPFub(sys);            break;
	default:     ibex_error("CellCostFunc::get_cost : error  wrong criterion.");
	             return NULL;
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------

CellCostVarLB::CellCostVarLB(const ExtendedSystem& sys, int ind_var) : CellCostFunc(sys, false), goal_var(ind_var) {

}

double CellCostVarLB::cost(const Cell& c) const {
	return c.box[goal_var].lb();
}


// -----------------------------------------------------------------------------------------------------------------------------------

CellCostVarUB::CellCostVarUB(const ExtendedSystem& sys, int ind_var) : CellCostFunc(sys, false), goal_var(ind_var) {

}

double CellCostVarUB::cost(const Cell& c) const {
	return c.box[goal_var].ub();
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostC3::CellCostC3(const ExtendedSystem& sys, double lb) : CellCostFunc(sys, true), loup(lb) {

}

double CellCostC3::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return -((loup - data->pf.lb()) / data->pf.diam() );
	} else {
		ibex_error("CellCostC3::cost : invalid cost");
		return POS_INFINITY;
	}
}

void CellCostC3::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostC3::set_optim_data(Cell& c) {
	((BxpOptimData*) c.prop[BxpOptimData::get_id(sys)])->compute_pf(*sys.goal,c.box);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostC5::CellCostC5(const ExtendedSystem& sys, double lb) : CellCostFunc(sys, true), loup(lb) {

}

double CellCostC5::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return (-(data->pu * (loup - data->pf.lb()) / data->pf.diam()));
	} else {
		ibex_error("CellCostC5::cost : invalid cost"); return POS_INFINITY;
	}
}

void CellCostC5::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostC5::set_optim_data(Cell& c) {
	BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	data->compute_pu(sys,c.box);
	data->compute_pf(*sys.goal,c.box);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostC7::CellCostC7(const ExtendedSystem& sys, int ind_var, double lb) : CellCostFunc(sys, true), loup(lb), goal_var(ind_var) {

}

double CellCostC7::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return c.box[goal_var].lb()/(data->pu*(loup-data->pf.lb())/data->pf.diam());
	} else {
		ibex_error("CellCostC7::cost : invalid cost"); return POS_INFINITY;
	}
}

void CellCostC7::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostC7::set_optim_data(Cell& c) {
	BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	data->compute_pu(sys,c.box);
	data->compute_pf(*sys.goal,c.box);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostPU::CellCostPU(const ExtendedSystem& sys) :  CellCostFunc(sys ,false) {

}

double CellCostPU::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return  -data->pu;
	} else {
		ibex_error("CellCostPU::cost : invalid cost"); return POS_INFINITY;
	}
}

void CellCostPU::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostPU::set_optim_data(Cell& c) {

	((BxpOptimData*) c.prop[BxpOptimData::get_id(sys)])->compute_pu(sys,c.box);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostPFlb::CellCostPFlb(const ExtendedSystem& sys) :  CellCostFunc(sys, false) {

}

void CellCostPFlb::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostPFlb::set_optim_data(Cell& c) {
	((BxpOptimData*) c.prop[BxpOptimData::get_id(sys)])->compute_pf(*sys.goal,c.box);
}

double CellCostPFlb::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return  data->pf.lb();
	} else {
		ibex_error("CellCostPFlb::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostPFub::CellCostPFub(const ExtendedSystem& sys) :  CellCostFunc(sys, false) {

}

void CellCostPFub::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostPFub::set_optim_data(Cell& c) {
	((BxpOptimData*) c.prop[BxpOptimData::get_id(sys)])->compute_pf(*sys.goal,c.box);
}

double CellCostPFub::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return data->pf.ub();
	} else {
		ibex_error("CellCostPFub::cost : invalid cost"); return POS_INFINITY;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

CellCostMaxPFub::CellCostMaxPFub(const ExtendedSystem& sys) :  CellCostFunc(sys, false) {

}

void CellCostMaxPFub::add_property(BoxProperties& map) {
	if (!map[BxpOptimData::get_id(sys)])
		map.add(new BxpOptimData(sys));
}

void CellCostMaxPFub::set_optim_data(Cell& c) {
	((BxpOptimData*) c.prop[BxpOptimData::get_id(sys)])->compute_pf(*sys.goal,c.box);
}

double CellCostMaxPFub::cost(const Cell& c) const {
	const BxpOptimData *data = (BxpOptimData*) c.prop[BxpOptimData::get_id(sys)];
	if (data) {
		return -data->pf.ub();
	} else {
		ibex_error("CellCostMaxPFub::cost : invalid cost");
		return POS_INFINITY;
	}
}

}  // end namespace ibex
