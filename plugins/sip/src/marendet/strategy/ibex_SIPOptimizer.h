/* ============================================================================
 * I B E X - ibex_SIPOptimizer.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_SIPOPTIMIZER_H__
#define __SIP_IBEX_SIPOPTIMIZER_H__

#include "ibex_Ctc.h"
#include "ibex_LoupFinderSIP.h"

#include "ibex_Bsc.h"
#include "ibex_Cell.h"
#include "ibex_CellBufferOptim.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

#include <iostream>

namespace ibex {
class SIPOptimizer {

public:
	enum Status {
		SUCCESS,
		TIMEOUT,
		INFEASIBLE,
		NO_FEASIBLE_FOUND,
		UNBOUNDED_OBJ,
		UNREACHED_PREC
	};

	static const double default_rel_eps_f;
	static const double default_abs_eps_f;
	static const double default_lf_loop_ratio;
	static const double default_eps_x;

	SIPOptimizer(int n, Ctc& ctc, Bsc& bisector,
			LoupFinder& loup_finder, LoupFinder& loup_finder2, CellBufferOptim& buffer, int goal_var,
			double eps_x=default_eps_x,
			double rel_eps_f=default_rel_eps_f,
			double abs_eps_f=default_abs_eps_f);

	SIPOptimizer::Status optimize(const IntervalVector& init_box, double obj_init_bound =
	POS_INFINITY);

	void report(bool verbose = true);
	double get_loup() const;
	double get_uplo() const;
	SIPOptimizer::Status get_status() const;
	IntervalVector get_loup_point() const;
	double get_time() const;
	double get_nb_cells() const;
	double get_obj_rel_prec() const;
	double get_obj_abs_prec() const;

	const int n;
	const int goal_var;
	int trace = 0;
	double timeout = -1;
	int maxiter = -1;

private:
	double compute_ymax();
	void handle_cell(Cell& c, const IntervalVector& init_box);
	void contract_and_bound(Cell& c, const IntervalVector& init_box);
	void updateUplo();
	bool updateLoup(Cell& cell);
	bool updateLoup2(Cell& cell);

	void updateUploEpsboxes(double ymin);

	Ctc& ctc_;
	Bsc& bisector_;
	LoupFinder& loup_finder_;
	LoupFinder& loup_finder2_;
	CellBufferOptim& buffer_;

	const double obj_rel_prec_f_;
	const double obj_abs_prec_f_;
	double eps_x_;
	double lf_loop_ratio_;

	SIPOptimizer::Status status_ = SIPOptimizer::Status::SUCCESS;
	double uplo_ = NEG_INFINITY;
	double loup_ = POS_INFINITY;
	double initial_loup_ = POS_INFINITY;
	double uplo_epsboxes = POS_INFINITY;
	double time_ = 0;
	IntervalVector loup_point_ = IntervalVector(1);
	int nb_cells_ = 0;
	bool loup_changed_ = false;

};

inline
    std::ostream& operator<<(std::ostream& os, SIPOptimizer::Status status) {
        switch(status) {
        case SIPOptimizer::Status::SUCCESS:
            os << "SUCCESS";
            break;
        case SIPOptimizer::Status::TIMEOUT:
            os << "TIMEOUT";
            break;
        case SIPOptimizer::Status::INFEASIBLE:
            os << "INFEASIBLE";
            break;
        case SIPOptimizer::Status::NO_FEASIBLE_FOUND:
            os << "NO FEASIBLE POINT FOUND";
            break;
        case SIPOptimizer::Status::UNREACHED_PREC:
            os << "UNREACHED PREC";
            break;
        case SIPOptimizer::Status::UNBOUNDED_OBJ:
            os << "UBOUNDED OBJ";
            break;
        }
        return os;
    }

} // end namespace ibex

#endif // __SIP_IBEX_SIPOPTIMIZER_H__
