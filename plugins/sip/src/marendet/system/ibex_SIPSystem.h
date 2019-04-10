/* ============================================================================
 * I B E X - ibex_SIPSystem.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_SIPSYSTEM_H__
#define __SIP_IBEX_SIPSYSTEM_H__

#include "ibex_Array.h"
#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_IntervalVector.h"
#include "ibex_NLConstraint.h"
#include "ibex_SIConstraint.h"
#include "ibex_SIConstraintCache.h"
#include "ibex_Bxp.h"

#include <iostream>
#include <utility>
#include <vector>
#include <regex>

namespace ibex {
class BxpNodeData;
class SIPSystem {

public:
	SIPSystem(const std::string& filename, const std::regex& quantified_regex);
	virtual ~SIPSystem();

	// Ibex system needed to parse the minibex file,
	// and must be kept alive because it is used
	// in the buffer
	System* ibex_system_;
	std::vector<SIConstraint> sic_constraints_;
	std::vector<IntervalVector> initial_parameter_boxes_;
	std::vector<NLConstraint> normal_constraints_;
	std::vector<Function*> constraints_functions_;
	Function* goal_function_;
	int nb_var; // nb_var without goal variable
	int ext_nb_var; // nb_var with goal variable
	double goal_ub(const IntervalVector& pt) const;
	bool is_inner(const IntervalVector& pt, BxpNodeData& prop) const;
	double max_constraints(const IntervalVector& pt, BxpNodeData& prop) const;
	// Load all node data for this node: current B&B box and SIConstraintsCache
	std::vector<SIConstraintCache> getInitialNodeCaches() const;
	//void loadBxpNodeData(BxpNodeData* BxpNodeData);
	//void updateBxpNodeData();
	IntervalVector extractInitialBox() const;

	//BxpNodeData* node_data_ = nullptr;

private:

	void extractConstraints();
	Array<const ExprSymbol> getVarSymbols(
			const Function* fun) const;
	Array<const ExprSymbol> getUsedParamSymbols(
			const Function* fun);
	Function* copyGoal();

	std::regex quantified_regex_;
};

class BxpNodeData: public Bxp {
public:
	//static SIPSystem* sip_system;
	static long id;
	//BxpNodeData();
	BxpNodeData(const std::vector<SIConstraintCache>& caches);
	virtual ~BxpNodeData() {}
	virtual Bxp* copy(const IntervalVector& box, const BoxProperties& prop) const;
	virtual void update(const BoxEvent& event, const BoxProperties& prop);
	//virtual std::string to_string() const;
	IntervalVector init_box;
	std::vector<SIConstraintCache> sic_constraints_caches;
	std::vector<SIConstraintCache> init_sic_constraints_caches;
};

} // end namespace ibex

#endif // __SIP_IBEX_SIPSYSTEM_H__
