////============================================================================
////                                  I B E X
//// File        : ibex_BxpLPMinimizer.cpp
//// Author      : Gilles Chabert
//// Copyright   : IMT Atlantique (France)
//// License     : See the LICENSE file
//// Created     : Jul 25, 2018
////============================================================================
//
//#include "ibex_BxpLPMinimizer.h"
//#include "ibex_Id.h"
//
//namespace ibex {
//
//Map<long,false> BxpLPMinimizer::ids;
//
//void BxpLPMinimizer::check() {
//	if (!up2date) {
////		assert(_active);
////		// TODO: if the impact contains no variable involved in the constraint
////		// we can avoid evaluation
////		Domain y=lin.right_hand_side();
////		switch (y.dim.type()) {
////		case Dim::SCALAR:       _active = !(lin.f.eval(box).is_subset(y.i())); break;
////		case Dim::ROW_VECTOR:
////		case Dim::COL_VECTOR:   _active = !(lin.f.eval_vector(box).is_subset(y.v())); break;
////		case Dim::MATRIX:       _active = !(lin.f.eval_matrix(box).is_subset(y.m())); break;
////		}
//	}
//	up2date=true;
//}
//
//void BxpLPMinimizer::update(const BoxEvent& e, const BoxProperties& prop) {
//	if (_active || e.type!=BoxEvent::CONTRACT) {
//		_active=true;
//		up2date=false;
//	}
//}
//
//long BxpLPMinimizer::get_id(const Linearizer& lin) {
//	try {
//		return ids[lin.id];
//	} catch(Map<long,false>::NotFound&) {
//		long new_id=next_id();
//		ids.insert_new(lin.id, new_id);
//		return new_id;
//	}
//}
//
//} // end namespace ibex
