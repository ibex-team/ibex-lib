/* ============================================================================
 * I B E X - ibex_SIPSystem.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_SIPSystem.h"

#include "ibex_CmpOp.h"
#include "ibex_ExprCopy.h"
#include "ibex_Interval.h"
#include "ibex_System.h"
#include "ibex_VarSet.h"
#include "ibex_Id.h"

using namespace std;

namespace ibex {

//SIPSystem* BxpNodeData::sip_system = nullptr;

long BxpNodeData::id = next_id();

/*BxpNodeData::BxpNodeData() :
		BxpNodeData(BxpNodeData::sip_system->getInitialNodeCaches()) {
}*/

BxpNodeData::BxpNodeData(const vector<SIConstraintCache>& caches) :
		Bxp(id), init_box(1), sic_constraints_caches(caches), init_sic_constraints_caches(caches) {
}

void BxpNodeData::update(const BoxEvent& event, const BoxProperties& prop) {
	// something should be done here, at least
	// an up-to-date flag should be set to "false".
}

Bxp* BxpNodeData::copy(const IntervalVector& box, const BoxProperties& prop) const {
	return new BxpNodeData(*this);
}

SIPSystem::SIPSystem(const string& filename, const regex& quantified_regex) :
		ibex_system_(new System(filename.c_str())), quantified_regex_(quantified_regex) {
	goal_function_ = copyGoal();
	extractConstraints();
	if(goal_function_ != NULL) {
		nb_var = goal_function_->nb_var();
	} else if (sic_constraints_.size() > 0) {
		nb_var = sic_constraints_[0].variable_count_;
	} else if (normal_constraints_.size() > 0) {
		nb_var = normal_constraints_[0].function_->nb_var();
	}
	//nb_var = goal_function_->nb_var();
	//ext_nb_var = nb_var+1;
	if (goal_function_ == NULL) {
		ext_nb_var = nb_var;
	} else {
		ext_nb_var = nb_var + 1;
	}
}

SIPSystem::~SIPSystem() {
	delete ibex_system_;
	while (!constraints_functions_.empty()) {
		delete constraints_functions_.back();
		constraints_functions_.pop_back();
	}
	if(goal_function_ != NULL)
		delete goal_function_;
}

double SIPSystem::goal_ub(const IntervalVector& pt) const {
	return goal_function_->eval(pt).ub();
}

bool SIPSystem::is_inner(const IntervalVector& pt, BxpNodeData& node_data) const {
	int sic_index = 0;
	for (const auto& sic : sic_constraints_) {
		if (sic.evaluateWithoutCachedValue(pt, node_data.sic_constraints_caches[sic_index]).ub() > 0) {
			return false;
		}
		sic_index++;
	}
	for (int i = 0; i < normal_constraints_.size() - (goal_function_ != NULL ? 1 : 0); ++i) {
		if (normal_constraints_[i].evaluate(pt).ub() > 0) {
			return false;
		}
	}
	return true;
}

double SIPSystem::max_constraints(const IntervalVector& pt, BxpNodeData& node_data) const {
	int sic_index = 0;
	Interval eval = Interval::EMPTY_SET;
	for (const auto& sic : sic_constraints_) {
		eval |= sic.evaluateWithoutCachedValue(pt, node_data.sic_constraints_caches[sic_index]);
		sic_index++;
	}
	for (int i = 0; i < normal_constraints_.size() - (goal_function_ != NULL ? 1 : 0); ++i) {
		eval |= normal_constraints_[i].evaluate(pt);
	}
	return eval.ub();
}

void SIPSystem::extractConstraints() {
	for (int i = 0; i < ibex_system_->ctrs.size(); ++i) {
		Array<const ExprSymbol> varSymbols = getVarSymbols(&ibex_system_->ctrs[i].f);
		Array<const ExprSymbol> usedParamSymbols = getUsedParamSymbols(&ibex_system_->ctrs[i].f);
		// Contains all variable symbols and only used parameter symbols
		Array<const ExprSymbol> allSymbols;
		allSymbols.add(varSymbols);
		allSymbols.add(usedParamSymbols);

		// Copy symbols for the new function with goal variable
		Array<const ExprSymbol> varCopy(varSymbols.size());
		varcopy(varSymbols, varCopy);
		Array<const ExprSymbol> usedParamCopy(usedParamSymbols.size());
		varcopy(usedParamSymbols, usedParamCopy);
		Array<const ExprSymbol> allCopiedSymbols;
		allCopiedSymbols.add(varCopy);
		allCopiedSymbols.add(usedParamCopy);
		// Copy the expression of the old function with new symbols,
		// and only the used parameters symbols (reduce the number of arguments
		// of the function)
		const ExprNode& copy = ExprCopy().copy(allSymbols, allCopiedSymbols,
				ibex_system_->ctrs[i].f.expr());
		Array<const ExprSymbol> allCopiedSymbolsWithObj;
		allCopiedSymbolsWithObj.add(varCopy);
		if (goal_function_ != NULL) {
			allCopiedSymbolsWithObj.add(ExprSymbol::new_("goal"));
		}
		allCopiedSymbolsWithObj.add(usedParamCopy);
		Function* newF = nullptr;
		if (ibex_system_->ctrs[i].op == CmpOp::LEQ)
			newF = new Function(allCopiedSymbolsWithObj, copy);
		else if (ibex_system_->ctrs[i].op == CmpOp::GEQ)
			newF = new Function(allCopiedSymbolsWithObj, -copy);
		// Add the function to the function pool
		constraints_functions_.push_back(newF);
		// Create NormalConstraint or SIConstraint depending on the presence of parameters
		if (usedParamSymbols.size() == 0) {
			normal_constraints_.push_back(NLConstraint(newF));
		} else {
			VarSet sicVarset(*newF, varCopy);
			// Extract the parameter box from the global initial box of the system
			VarSet tmpVarset(ibex_system_->ctrs[i].f, usedParamSymbols, false);
			IntervalVector initParamBox = tmpVarset.param_box(ibex_system_->box);
			initial_parameter_boxes_.emplace_back(initParamBox);
			int ext_var_dim = newF->nb_var() - initParamBox.size();
			if (goal_function_ != NULL) {
				SIConstraint test(newF, ext_var_dim);
				sic_constraints_.emplace_back(test);
			} else {
				SIConstraint test(newF, ext_var_dim-1);
				sic_constraints_.emplace_back(test);
			}
		}
	}

	// Goal constraint
	if (goal_function_) {
		int nb_arg = goal_function_->nb_arg() + 1;
		Array<const ExprSymbol> goal_ctr_args(nb_arg);
		goal_ctr_args.set_ref(nb_arg - 1, ExprSymbol::new_("goal"));
		varcopy(goal_function_->args(), goal_ctr_args);
		const ExprNode& goal_expr = ExprCopy().copy(goal_function_->args(), goal_ctr_args,
				goal_function_->expr());

		Function* goalF = new Function(goal_ctr_args, goal_expr - goal_ctr_args[nb_arg - 1]);
		constraints_functions_.push_back(goalF);
		normal_constraints_.push_back(NLConstraint(goalF));
	}
}

/*
 * Return an ibex array containing the variable symbols of a function
 */
Array<const ExprSymbol> SIPSystem::getVarSymbols(const Function* fun) const {
	Array<const ExprSymbol> varSymbols;
	for (int i = 0; i < fun->nb_arg(); ++i) {
		//if (!(fun->arg_name(i)[0] == 'p' && fun->arg_name(i)[1] == '_')) {
		string name = fun->arg_name(i);
		if(!regex_match(name.begin(), name.end(), quantified_regex_)) {
			varSymbols.add(fun->arg(i));
		}
	}
	return varSymbols;
}

Function* SIPSystem::copyGoal() {
	if (ibex_system_->goal == NULL) {
		return NULL;
	}
	Array<const ExprSymbol> varSymbols = getVarSymbols(ibex_system_->goal);
	Array<const ExprSymbol> varCopy(varSymbols.size());
	varcopy(varSymbols, varCopy);
	const ExprNode& copy = ExprCopy().copy(varSymbols, varCopy, ibex_system_->goal->expr());
	Function* newGoal = new Function(varCopy, copy);
	return newGoal;
}

/*
 * Return an ibex array containing only the used parameter symbols of a function.
 * A parameter symbol is a symbol prefixed with p_.
 */
Array<const ExprSymbol> SIPSystem::getUsedParamSymbols(const Function* fun) {
	Array<const ExprSymbol> paramSymbols;
	int current_var = 0;
	for (int i = 0; i < fun->nb_arg(); ++i) {
		//if (fun->arg_name(i)[0] == 'p' && fun->arg_name(i)[1] == '_' && fun->used_vars[i]) {
		string name = fun->arg_name(i);
		bool used = std::find(fun->used_vars.begin(), fun->used_vars.end(), current_var)
			!= fun->used_vars.end();
		if(used && regex_match(name.begin(), name.end(), quantified_regex_)) {
			paramSymbols.add(fun->arg(i));
		}
		current_var += fun->arg(i).dim.size();
	}
	return paramSymbols;
}

/*
 * Return the initial box of the variable symbols
 */
IntervalVector SIPSystem::extractInitialBox() const {
	// All var symbols
	Array<const ExprSymbol> varSymbols = getVarSymbols(&ibex_system_->f_ctrs);
	VarSet tmp(ibex_system_->f_ctrs, varSymbols);
	return tmp.var_box(ibex_system_->box);
}

vector<SIConstraintCache> SIPSystem::getInitialNodeCaches() const {
	vector<SIConstraintCache> caches;
	for (const IntervalVector& parameter_box : initial_parameter_boxes_) {
		caches.emplace_back(SIConstraintCache(parameter_box));
	}
	return caches;
}

/*void SIPSystem::loadBxpNodeData(BxpNodeData* node_data) {
	node_data_ = node_data;
	updateBxpNodeData();
}

void SIPSystem::updateBxpNodeData() {
	for (int i = 0; i < sic_constraints_.size(); ++i) {
		sic_constraints_[i].loadCache(&node_data_->sic_constraints_caches[i]);
	}
}*/
} // end namespace ibex
