/* ============================================================================
 * I B E X - ibex_LoupFinderLineSearch.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 12, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_LOUPFINDERLINESEARCH_H__
#define __SIP_IBEX_LOUPFINDERLINESEARCH_H__

#include "ibex_RelaxationLinearizerSIP.h"
#include "ibex_RestrictionLinearizerSIP.h"
#include "ibex_SIPSystem.h"

#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"
#include "ibex_LPSolver.h"
#include "ibex_LoupFinderSIP.h"

#include <utility>
#include <set>

namespace ibex {
class LoupFinderLineSearch: public LoupFinderSIP {
public:

	static const double default_sigma;
	enum InnerPointStrategy {
		STEIN, ACTIVE_RELAXATIONS, ALL_RELAXATIONS, BLANKENSHIP, MIDPOINT, CORNER
	};
	LoupFinderLineSearch(const SIPSystem& system, const std::set<InnerPointStrategy>& strategies);
	virtual ~LoupFinderLineSearch();

	

	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);
	bool is_inner_with_paving_simplification(const IntervalVector& box, const BxpNodeData* local_node_data, int kmax=4);
private:
	std::set<InnerPointStrategy> strategies_;
	RelaxationLinearizerSIP linearizer_;
	LPSolver lp_solver_;
	Vector relax_point_;
	LPSolver dir_solver_;
	LPSolver* corner_solver_;
	RestrictionLinearizerSIP* corner_linearizer_;
	double sigma_;
	IntervalVector box_;
	IntervalVector ext_box_;
	BxpNodeData* node_data_ = nullptr;
	BoxProperties* prop_ = nullptr;
	const BxpNodeData* initial_node_data_ = nullptr;
	bool delete_node_data_ = false;

	bool relaxations_direction(Vector& direction, double& obj, bool actives_only, bool with_sides=false);
	bool blankenship_direction(Vector& direction, double& obj);
	bool stein_direction(Vector&, double& obj);
	Interval t_value(const Vector& direction);
	bool line_search(const Vector& start_point, const Vector& end_point, Vector& loup_point, double& loup);

	bool do_strategy(InnerPointStrategy strategy);
	bool corner_restrictions(Vector& loup_point);

};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERLINESEARCH_H__

