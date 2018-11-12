//============================================================================
//                                  I B E X                                   
// File        : ibex_SIPOptimizer.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_SIPOptimizer.h"

#include "ibex_Ctc.h"
#include "ibex_LoupFinderSIP.h"
#include "ibex_SIPSystem.h"
#include "ibex_utils.h"

#include "ibex_BisectionPoint.h"
#include "ibex_Cell.h"
#include "ibex_CellBuffer.h"
#include "ibex_CellBufferOptim.h"
#include "ibex_Exception.h"
#include "ibex_LoupFinder.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_Timer.h"
#include "ibex_Vector.h"


#include <cmath>
#include <iomanip>
#include <utility>

using namespace std;

namespace ibex {

const double SIPOptimizer::default_rel_eps_f = 1e-3;
const double SIPOptimizer::default_abs_eps_f = 1e-3;
const double SIPOptimizer::default_lf_loop_ratio = 0.1;

SIPOptimizer::SIPOptimizer(Ctc& ctc, Bsc& bisector, LoupFinderSIP& loup_finder, LoupFinderSIP* loup_finder2,
		CellBufferOptim& buffer, double abs_eps, double rel_eps, double timeout, double eps_x, int maxiter, double lf_loop_ratio) :
		ctc_(ctc), bisector_(bisector), loup_finder_(loup_finder), loup_finder2_(loup_finder2), buffer_(buffer), obj_rel_prec_f_(
				rel_eps), obj_abs_prec_f_(abs_eps), timeout_(timeout), eps_x_(eps_x), maxiter_(maxiter), lf_loop_ratio_(lf_loop_ratio) {

}

SIPOptimizer::Status SIPOptimizer::optimize(const IntervalVector& box, double obj_init_bound) {
	n = box.size() + 1;
	loup_ = obj_init_bound;
	// Initialize the loup for the buffer
	buffer_.contract(loup_);
	uplo_ = NEG_INFINITY;
	uplo_epsboxes = POS_INFINITY;
	nb_cells_ = 0;
	int iter = 0;
	buffer_.flush();

	IntervalVector initial_box(n);
	initial_box.put(0, box);
	initial_box[n - 1] = Interval::ALL_REALS;
	Cell* root = new Cell(initial_box);
	//root->prop.add(new BxpNodeData());
	bisector_.add_property(initial_box, root->prop);
	buffer_.add_property(initial_box, root->prop);
	ctc_.add_property(initial_box, root->prop);
	loup_changed_ = false;
	initial_loup_ = obj_init_bound;
	loup_point_ = box;
	time_ = 0;
	Timer timer;
	timer.start();
	handleCell(*root);
	updateUplo();

	cout << setprecision(12);
	//maxiter_ = 7;
	while (!buffer_.empty() && (timeout_ <= 0 || time_ < timeout_) && (maxiter_ < 0 || iter < maxiter_)) {
		++iter;
		loup_changed_ = false;
		Cell* cell = buffer_.top();
		if (trace_ >= 1) {
			cout << "******** ITERATION " << iter << "********" << endl;
			cout << " current box " << cell->box << endl;
		}
		try {
            /*BisectionPoint bisection_point = bisector_.choose_var(*cell);
           auto new_cells = cell->bisect(bisection_point);*/
		    std::pair<Cell*,Cell*> new_cells = bisector_.bisect(*cell);
			buffer_.pop();
			delete cell;
			nb_cells_ += 2;
			handleCell(*new_cells.first);
			handleCell(*new_cells.second);
			//handleCell(*cell);
		} catch (NoBisectableVariableException&) {
			updateUploEpsboxes((cell->box)[n].lb());
			buffer_.pop();
			delete cell;
			updateUplo();
			continue;
		}

		if (uplo_epsboxes == NEG_INFINITY) {
			cout << " possible infinite minimum " << endl;
			break;
		}
		if (loup_changed_) {
			double ymax = compute_ymax();
			buffer_.contract(ymax);
			if (ymax <= NEG_INFINITY) {
				if (trace_ > 0) {
					cout << " infinite value for the minimum " << endl;
					break;
				}
			}
		}
		updateUplo();
		if (timeout_ > 0) {
			timer.check(timeout_);
		}
		time_ = timer.get_time();
	}
	time_ = timer.get_time();
	timer.stop();
	if (timeout_ > 0 && time_ > timeout_) {
		status_ = Status::timeout;
	} else if (uplo_epsboxes == POS_INFINITY
			&& (loup_ == POS_INFINITY || (loup_ == initial_loup_ && obj_abs_prec_f_ == 0 && obj_rel_prec_f_ == 0))) {
		status_ = Status::infeasible;
	} else if (loup_ == initial_loup_) {
		status_ = Status::no_feasible_found;
	} else if (uplo_epsboxes == NEG_INFINITY) {
		status_ = Status::unbounded_obj;
	} else if (get_obj_rel_prec() > obj_rel_prec_f_ && get_obj_abs_prec() > obj_abs_prec_f_) {
		status_ = Status::unreached_prec;
	} else {
		status_ = Status::success;
	}
	return status_;
}

void SIPOptimizer::handleCell(Cell& cell) {
	// LOAD THE NEW CACHE!

	//BxpNodeData* data=(BxpNodeData*) cell.prop[BxpNodeData::id];
	//if (data == nullptr) ibex_error("[ibexopt-sip]: no node data!");

	//BxpNodeData::sip_system->loadBxpNodeData(data);
	//cout << "before : " << cell.box << endl;
	//const auto& list = cell.get<BxpNodeData>().sic_constraints_caches[0].parameter_caches_;
	/*cout << "{";
	for(const auto& param_cache : list) {
		cout << "{";;
		cout << "{" << param_cache.parameter_box[0].lb() << "," << param_cache.parameter_box[0].ub() << "}";
		for(int i = 1; i < param_cache.parameter_box.size(); ++i) {
			cout << ",{" << param_cache.parameter_box[i].lb() << "," << param_cache.parameter_box[i].ub() << "}";
		}
		cout << "},";
		//cout << param_cache.parameter_box << " eval=" << param_cache.evaluation <<  endl;
	}
	cout << "}" << endl;*/
	//cout << "paramsize=" << list.size() << endl;
	/*for(int i = 0; i < 1; ++i) {
		const auto& list = cell.get<BxpNodeData>().sic_constraints_caches[i].parameter_caches_;
		cout << "paramsize[" << i << "]=" << list.size() << endl;
	}*/
	//cout << endl << endl;



	ContractContext context(cell.prop);
	/*if (cell.bisected_var!=-1) {
		context.impact.clear();
		context.impact.add(cell.bisected_var);
		context.impact.add(cell.box.size()-1);
	}*/

	// =============== Contract y with y <= loup
	Interval& y = cell.box[n - 1];
	double ymax;
	if (loup_ == POS_INFINITY)
		ymax = POS_INFINITY;
	else
		ymax = compute_ymax();

	y &= Interval(NEG_INFINITY, ymax);
	if (y.is_empty()) {
		cell.box.set_empty();
	}

	// =============== Contract x with f(x) = y and g(x) <= 0
	bool loop = true;
	while (!cell.box.is_empty() && loop) {
		//cout << "after: " << cell.box << endl << endl << endl;
		//cout << "beforectc" << endl;
		double old_loup = loup_;
		ctc_.contract(cell.box, context);
		//cout << "afterctc" << endl;
		loop = false;

		if (!cell.box.is_empty()) {
			bool loup_changed_here = updateLoup2(cell);
			loop = loup_changed_here;
			if (loup_changed_here) {
				y &= Interval(NEG_INFINITY, compute_ymax());
			}
			loup_changed_ |= loup_changed_here;
			if (y.is_empty()) {
				cell.box.set_empty();
			}
		}
		loop = (old_loup - loup_)/loup_ > lf_loop_ratio_;
		//break;
	}


	// =============== Update loup
	// we pass the full box with goal to the updateLoup function,
	// the linearize function won't use the last variable of the box
	//bool loup_changed_here = updateLoup(box_without_gaol);
	if (!cell.box.is_empty()) {
		bool loup_changed_here = updateLoup(cell);
		if (loup_changed_here) {
			y &= Interval(NEG_INFINITY, compute_ymax());
		}
		loup_changed_ |= loup_changed_here;
		if (y.is_empty()) {
			cell.box.set_empty();
		}
	}


	// =============== Handle epsilon boxes
	if (!cell.box.is_empty()) {
		IntervalVector box_without_gaol = cell.box.subvector(0, n - 2);
		if ((box_without_gaol.max_diam() <= eps_x_ && y.diam() <= obj_abs_prec_f_) || !cell.box.is_bisectable()) {
			updateUploEpsboxes(y.lb());
			cell.box.set_empty();
		}
	}

	if (cell.box.is_empty()) {
		delete &cell;
	} else {
		//cell.box.put(0, box_without_gaol); // TODO Useful ?
		buffer_.push(&cell);
	}
}

void SIPOptimizer::updateUplo() {
	double new_uplo = POS_INFINITY;

	if (!buffer_.empty()) {
		new_uplo = buffer_.minimum();
		if (new_uplo > loup_) {
			cout << " loup = " << loup_ << " new_uplo=" << new_uplo << endl;
			ibex_error("optimizer: new_uplo>loup (please report bug)");
		}
		if (new_uplo < uplo_) {
			cout << "uplo= " << uplo_ << " new_uplo=" << new_uplo << endl;
			ibex_error("optimizer: new_uplo<uplo (please report bug)");
		}
		if (new_uplo < uplo_epsboxes) {
			if (new_uplo > uplo_) {
				uplo_ = new_uplo;
				if (trace_ > 0) {
					cout << "\033[33m uplo= " << uplo_ << "\033[0m" << endl;
				}
			}
		} else {
			uplo_ = uplo_epsboxes;
		}
	} else if (buffer_.empty() && loup_ != POS_INFINITY) {
		new_uplo = compute_ymax();
		double m = (new_uplo < uplo_epsboxes) ? new_uplo : uplo_epsboxes;
		if (uplo_ < m) {
			uplo_ = m;
		}
	}
}

bool SIPOptimizer::updateLoup(Cell& cell) {
	if (cell.box.is_empty())
		return false;
	try {
		auto p = loup_finder_.find(sip_from_ext_box(cell.box), loup_point_, loup_, cell.prop);
		loup_point_ = p.first; // -2 to remove the goal variable
		loup_ = p.second;
		if (trace_ > 0) {
			cout << "                    ";
			cout << "\033[32m loup= " << loup_ << "(lf1)\033[0m" << endl;

		}
		return true;
	} catch (LoupFinder::NotFound&) {
		return false;
	}
}

bool SIPOptimizer::updateLoup2(Cell& cell) {
	if (cell.box.is_empty() || loup_finder2_ == nullptr)
		return false;
	try {
		auto p = loup_finder2_->find(sip_from_ext_box(cell.box), loup_point_, loup_, cell.prop);
		loup_point_ = p.first; // -2 to remove the goal variable
		loup_ = p.second;
		if (trace_ > 0) {
			cout << "                    ";
			cout << "\033[32m loup= " << loup_ << " (lf2)\033[0m" << endl;

		}
		return true;
	} catch (LoupFinder::NotFound&) {
		return false;
	}
}

void SIPOptimizer::updateUploEpsboxes(double ymin) {
	if (uplo_epsboxes > ymin) {
		uplo_epsboxes = ymin;
		if (trace_ > 0) {
			cout << " unprocessable tiny box: now uplo <=" << setprecision(12) << uplo_epsboxes << " uplo= " << uplo_
					<< endl;
		}
	}
}

void SIPOptimizer::report(bool verbose) {
	if (!verbose) {
		cout << get_status() << " ";
		cout << time_ << " ";
		cout << nb_cells_ << " ";
		cout << get_uplo() << ' ' << get_loup() << " ";
		cout << get_loup_point() << endl;
		return;
	}
	switch (status_) {
	case success:
		cout << "\033[32m" << " optimization successful!" << endl;
		break;
	case infeasible:
		cout << "\033[31m" << " infeasible problem" << endl;
		break;
	case no_feasible_found:
		cout << "\033[31m" << " no feasible point found (the problem may be infeasible)" << endl;
		break;
	case unbounded_obj:
		cout << "\033[31m" << " possibly unbounded objective (f*=-oo)" << endl;
		break;
	case timeout:
		cout << "\033[31m" << " time limit " << timeout_ << "s. reached " << endl;
		break;
	case unreached_prec:
		cout << "\033[31m" << " unreached precision" << endl;
	}

	cout << "\033[0m" << endl;

	// No solution found and optimization stopped with empty buffer  before the required precision is reached => means infeasible problem
	if (buffer_.empty() && uplo_epsboxes == POS_INFINITY
			&& (loup_ == POS_INFINITY || (loup_ == initial_loup_ && obj_abs_prec_f_ == 0 && obj_rel_prec_f_ == 0))) {
		cout << " infeasible problem " << endl;
	} else {
		cout << " best bound in: [" << uplo_ << "," << loup_ << "]" << endl;

		double rel_prec = get_obj_rel_prec();
		double abs_prec = get_obj_abs_prec();

		cout << " relative precision obtained on objective function: " << rel_prec << " "
				<< (rel_prec <= obj_rel_prec_f_ ? " [passed]" : " [failed]") << endl;

		cout << " absolute precision obtained on objective function: " << abs_prec << " "
				<< (abs_prec <= obj_abs_prec_f_ ? " [passed]" : " [failed]") << endl;

		if (loup_ == initial_loup_)
			cout << " no feasible point found " << endl;
		else {
			cout << " best feasible point: ";

			cout << loup_point_.lb() << endl;
		}
	}
	cout << " cpu time used: " << time_ << "s." << endl;
	cout << " number of cells: " << nb_cells_ << endl;

}

double SIPOptimizer::get_loup() const {
	return loup_;
}

double SIPOptimizer::get_uplo() const {
	return uplo_;
}

SIPOptimizer::Status SIPOptimizer::get_status() const {
	return status_;
}

IntervalVector SIPOptimizer::get_loup_point() const {
	return loup_point_;
}

double SIPOptimizer::get_time() const {
	return time_;
}

double SIPOptimizer::get_nb_cells() const {
	return nb_cells_;
}

double SIPOptimizer::get_obj_abs_prec() const {
	return loup_ - uplo_;
}

double SIPOptimizer::get_obj_rel_prec() const {
	if (loup_ == POS_INFINITY)
		return POS_INFINITY;
	else if (loup_ == 0)
		if (uplo_ < 0)
			return POS_INFINITY;
		else
			return 0;
	else
		return (loup_ - uplo_) / (fabs(loup_));

}

double SIPOptimizer::compute_ymax() {
	double ymax = loup_ - obj_rel_prec_f_ * fabs(loup_);
	if (loup_ - obj_abs_prec_f_ < ymax)
		ymax = loup_ - obj_abs_prec_f_;
	return ymax;
}

} // end namespace ibex
