/* ============================================================================
 * I B E X - ibex_SICPaving.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : July 13, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_SICPaving.h"

#include "ibex_SIPSystem.h"

#include "ibex_utils.h"
#include "ibex_Newton.h"
#include "ibex_Linear.h"

namespace ibex {

void bisect_paving(SIConstraintCache& cache) {
	const int cache_size = cache.parameter_caches_.size();
	for(int i = 0; i < cache_size; ++i) {
		auto bisects = bisectAllDim(cache.parameter_caches_[i].parameter_box);
		cache.parameter_caches_[i] = ParameterEvaluationsCache(bisects[0]);
		for(int j = 1; j < bisects.size(); ++j) {
			cache.parameter_caches_.emplace_back(ParameterEvaluationsCache(bisects[j]));
		}
	}
}

bool is_feasible_with_paving(const SIConstraint& constraint, const SIConstraintCache& cache, const IntervalVector& box) {
	const auto& list = cache.parameter_caches_;
	for(int i = 0; i < list.size(); ++i) {
		if(constraint.evaluate(box, list[i].parameter_box).ub() > 0) {
			return false;
		}
	}
	return true;
}

void simplify_paving(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box, bool with_newton) {
    cache.update_cache(*constraint.function_, box, true);
	monotonicity_filter(constraint, cache, box);
    evaluation_filter(constraint, cache, box);
    if(with_newton) {
        newton_filter(constraint, cache, box);
    }
}


void monotonicity_filter(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box) {
	auto& list = cache.parameter_caches_;
    auto it = list.begin();
    IntervalVector paramBoxUnion = cache.initial_box_;
    while (it != list.end()) {
		bool keepInVector = true;
		for (int paramCount = 0; paramCount < it->parameter_box.size(); ++paramCount) {
			int paramIndex = paramCount + constraint.variable_count_;
			if (it->full_gradient[paramIndex].lb() > 0) {
				it->parameter_box[paramCount] = it->parameter_box[paramCount].ub();
				if (it->parameter_box[paramCount].ub() < paramBoxUnion[paramCount].ub()) {
					keepInVector = false;
				} else {
					// TODO Maybe update only at the end
					//_updateMemoryBox(*it);
				}
			} else if (it->full_gradient[paramIndex].ub() < 0) {
				it->parameter_box[paramCount] = it->parameter_box[paramCount].lb();
				if (it->parameter_box[paramCount].lb() > paramBoxUnion[paramCount].lb()) {
					keepInVector = false;
				} else {
					//_updateMemoryBox(*it);
				}
			}
		}
		if (!keepInVector) {
			it = list.erase(it);
		} else {
			//_updateMemoryBox(*it);
			++it;
		}
	}
}

void evaluation_filter(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box) {
    auto& list = cache.parameter_caches_;
    auto it = list.begin();
	while (it != list.end()) {
		if (constraint.evaluate(box, it->parameter_box).ub() <= 0) {
			it = list.erase(it);
		} else {
			it++;
		}
	}
}

void newton_filter(const SIConstraint& constraint, SIConstraintCache& cache, const IntervalVector& box) {
    auto& list = cache.parameter_caches_;
    auto it = list.begin();
    IntervalVector paramBoxUnion = cache.initial_box_;
    IntervalVector fullbox(constraint.variable_count_ + constraint.parameter_count_);
    //fullbox.put(0, box);
    BitSet bitset(fullbox.size());
    for(int i = constraint.variable_count_; i < fullbox.size(); ++i) {
        bitset.add(i);
    }
    VarSet vars(fullbox.size(), bitset);

	//IntervalVector param_gradient(vars.nb_var);
	IntervalMatrix full_hessian(vars.nb_var, fullbox.size());
	IntervalMatrix param_hessian(vars.nb_var, vars.nb_var);
	IntervalVector h(vars.nb_var);


    while(it != list.end()) {
        if(!it->parameter_box.is_interior_subset(paramBoxUnion)) {
			it++;
            continue;
        }
		fullbox = vars.full_box(it->parameter_box, box);
		IntervalVector fullbox_mid = vars.full_box(it->parameter_box.mid(), box);
		IntervalVector param_gradient = vars.var_box(constraint.function_->gradient(fullbox_mid));
		for(int i = 0; i < vars.nb_var; ++i) {
			constraint.function_->diff().jacobian(fullbox, full_hessian, bitset, vars.var(i));
		}
		for(int i = 0; i < vars.nb_var; ++i) {
			param_hessian.set_row(i, vars.var_box(full_hessian.row(i)));
		}
		h = it->parameter_box - it->parameter_box.mid();
		try {
			precond(param_hessian, param_gradient);
		} catch(...) {
			//ibex_warning("Precond failed in newton_filter");
		}
		gauss_seidel(param_hessian, -param_gradient, h);
		it->parameter_box = h + it->parameter_box.mid();



		//std::cout << "box=" << box << std::endl;
        //fullbox.put(constraint.variable_count_, it->parameter_box);
		//std::cout << fullbox.size() << std::endl;
		//std::cout << vars << std::endl;

		//std::cout << constraint.function_->diff() << std::endl;
        //bool has_contracted = newton(constraint.function_->diff(), vars, fullbox);
        if(h.is_empty()) {
            it = list.erase(it);
        } else {
            it++;
        }
    }
}

void blankenship(const IntervalVector& box, const SIPSystem& sys, BxpNodeData* node_data) {
	//std::cout << "xlin(blankenship)=" << box << std::endl;
	//BxpNodeData node_data_copy = BxpNodeData(*sys.node_data_);
	BxpNodeData node_data_copy = BxpNodeData(*node_data);
	for(int cst_index = 0; cst_index < sys.sic_constraints_.size(); ++cst_index) {
		const auto& sic = sys.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		//cache.parameter_caches_.clear();
		//cache.parameter_caches_.emplace_back(ParameterEvaluationsCache(cache.initial_box_));
		simplify_paving(sys.sic_constraints_[cst_index], cache, box, true);
	}
	const int MAX_ITERATIONS = 4;
	for(int i = 0; i < MAX_ITERATIONS; ++i) {
		for(int cst_index = 0; cst_index < sys.sic_constraints_.size(); ++cst_index) {
			const auto& sic = sys.sic_constraints_[cst_index];
			auto& cache = node_data_copy.sic_constraints_caches[cst_index];
			bisect_paving(cache);
			
			//monotonicity_filter(sys.sic_constraints_[cst_index], cache, box);
			//newton_filter(sys.sic_constraints_[cst_index], cache, box);

			simplify_paving(sys.sic_constraints_[cst_index], cache, box, true);

			//cache.update_cache(*sys.sic_constraints_[cst_index].function_, box, true);
			//monotonicity_filter(sys.sic_constraints_[cst_index], cache, box);
    		//evaluation_filter(sys.sic_constraints_[cst_index], cache, box);
        	//newton_filter(sys.sic_constraints_[cst_index], cache, box);
		}
	}
	//std::cout << "NEW ITER" << std::endl << std::endl;
	for(int cst_index = 0; cst_index < sys.sic_constraints_.size(); ++cst_index) {
		const auto& sic = sys.sic_constraints_[cst_index];
		auto& cache = node_data_copy.sic_constraints_caches[cst_index];
		const int max_blankenship_list_size = 2 * sic.variable_count_;
		//auto& blankenship_list = sys.node_data_->sic_constraints_caches[cst_index].best_blankenship_points_;
		auto& blankenship_list = node_data->sic_constraints_caches[cst_index].best_blankenship_points_;
		//std::cout << cache.parameter_caches_.size() << std::endl;
		//blankenship_list.clear();
		for(const auto& param_box : cache.parameter_caches_) {
			if(std::find(blankenship_list.begin(), blankenship_list.end(), param_box.parameter_box.mid()) == blankenship_list.end()) {
				//std::cout << "blankenship=" << param_box.parameter_box << std::endl;
				blankenship_list.emplace_back(param_box.parameter_box.mid());
				//blankenship_list.emplace_back(param_box.parameter_box.mid());
			}
		}
		while(blankenship_list.size() > max_blankenship_list_size) {
			blankenship_list.pop_front();
		}
	}
}


/*FncGradientSubset::FncGradientSubset(const Function& f, const BitSet& bitset) : Fnc(f.nb_vars(), bitset.size()), components_(bitset), f_(f) {}
IntervalVector FncGradientSubset::eval_vector(const IntervalVector& box, const BitSet& components) const {
	IntervalVector res(components.size());
	int current_absolute_row = bitset.min()
	for(int i = 0; i < components.size(); ++i) {
		f->diff()[current_abs]
	}
}
void FncGradientSubset::jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v=-1) const;
*/
}