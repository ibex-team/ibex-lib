//============================================================================
//                                  I B E X                                   
// File        : ibex_SIPSystem.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_SIPSystem.h"

#include "ibex_CmpOp.h"
#include "ibex_ExprCopy.h"
#include "ibex_Interval.h"
#include "ibex_System.h"
#include "ibex_VarSet.h"


using namespace std;

namespace ibex {

SIPSystem* NodeData::sip_system = nullptr;

NodeData::NodeData() :
		NodeData(NodeData::sip_system->getInitialNodeCaches()) {
}

NodeData::NodeData(const vector<SIConstraintCache>& caches) :
		sic_constraints_caches(caches) {
}

std::pair<Backtrackable*, Backtrackable*> NodeData::down(const BisectionPoint& bp) {
	return std::pair<Backtrackable*, Backtrackable*>(new NodeData(*this), new NodeData(*this));
}

Backtrackable* NodeData::copy() const {
	return new NodeData(*this);
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

bool SIPSystem::is_inner(const IntervalVector& pt) const {
	for (const auto& sic : sic_constraints_) {
		if (sic.evaluateWithoutCachedValue(pt).ub() > 0) {
			return false;
		}
	}
	for (int i = 0; i < normal_constraints_.size() - 1; ++i) {
		if (normal_constraints_[i].evaluate(pt).ub() > 0) {
			return false;
		}
	}
	return true;
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
			if (goal_function_ != NULL) {
				SIConstraint test(newF, varCopy.size() + 1);
				sic_constraints_.emplace_back(test);
			} else {
				SIConstraint test(newF, varCopy.size());
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
	for (int i = 0; i < fun->nb_arg(); ++i) {
		//if (fun->arg_name(i)[0] == 'p' && fun->arg_name(i)[1] == '_' && fun->used(i)) {
		string name = fun->arg_name(i);
		if(fun->used(i) && regex_match(name.begin(), name.end(), quantified_regex_)) {
			paramSymbols.add(fun->arg(i));
		}
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

vector<SIConstraintCache> SIPSystem::getInitialNodeCaches() {
	vector<SIConstraintCache> caches;
	for (const IntervalVector& parameter_box : initial_parameter_boxes_) {
		caches.emplace_back(SIConstraintCache(parameter_box));
	}
	return caches;
}

void SIPSystem::loadNodeData(NodeData* node_data) {
	node_data_ = node_data;
	updateNodeData();
}

void SIPSystem::updateNodeData() {
	for (int i = 0; i < sic_constraints_.size(); ++i) {
		sic_constraints_[i].loadCache(&node_data_->sic_constraints_caches[i]);
	}
}
} // end namespace ibex
