/* ============================================================================
 * I B E X - ibex_CtcFirstOrderTest.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_CtcFirstOrderTest.h"

#include "ibex_Function.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_IntervalVector.h"
#include "ibex_Linear.h"
#include "ibex_LinearException.h"
#include "ibex_Vector.h"

#include <vector>

using namespace std;

namespace ibex {

CtcFirstOrderTest::CtcFirstOrderTest(const SIPSystem& system) :
		Ctc(system.ext_nb_var), system_(system) {

}

CtcFirstOrderTest::~CtcFirstOrderTest() {
}

void CtcFirstOrderTest::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(system_.getInitialNodeCaches()));
        ((BxpNodeData*)map[BxpNodeData::id])->init_box = init_box;
    }
}

void CtcFirstOrderTest::contract(IntervalVector& box) {
    ibex_warning("CtcFirstOrderTest: called with no context");

}
void CtcFirstOrderTest::contract(IntervalVector& box, ContractContext& context) {
	if(box.size() == 2) {
		return;
	}
	BxpNodeData* node_data = (BxpNodeData*) context.prop[BxpNodeData::id];
	if(node_data == nullptr) {
		ibex_error("CtcFirstOrderTest: BxpNodeData must be set");
	}
	vector<IntervalVector> gradients;
	for (int i = 0; i < system_.normal_constraints_.size() - 1; ++i) {
		if (!system_.normal_constraints_[i].isSatisfied(box)) {
			gradients.push_back(system_.normal_constraints_[i].gradient(box));
		}
	}
	for (int i = 0; i < system_.sic_constraints_.size(); ++i) {
		if (!system_.sic_constraints_[i].isSatisfied(box, node_data->sic_constraints_caches[i])) {
			gradients.push_back(system_.sic_constraints_[i].gradient(box, node_data->sic_constraints_caches[i]));
		}
	}

	// Without the goal variable
	IntervalMatrix matrix(nb_var - 1, gradients.size() + 1);
	matrix.set_col(0, system_.goal_function_->gradient(box.subvector(0, nb_var - 2)));
	for (int i = 0; i < gradients.size(); ++i) {
		matrix.set_col(i + 1, gradients[i].subvector(0, nb_var - 2));
	}
	bool testfailed = true;
	if (matrix.nb_cols() == 1) {
		if (matrix.col(0).contains(Vector::zeros(nb_var - 2))) {
			testfailed = false;
		}
	} else {
		int* pr = new int[matrix.nb_rows()];
		int* pc = new int[matrix.nb_cols()];
		IntervalMatrix LU(matrix.nb_rows(), matrix.nb_cols());
		testfailed = true;
		try {
			interval_LU(matrix, LU, pr, pc);
		} catch(SingularMatrixException&) {
			testfailed = false;
		}
		delete[] pr;
		delete[] pc;
	}
	if(testfailed) {
		box.set_empty();
	}

}
} // end namespace ibex
