////============================================================================
////                                  I B E X
//// File        : ibex_CellBufferOptim.h
//// Author      : Gilles Chabert, Bertrand Neveu
//// Copyright   : IMT Atlantique (France)
//// License     : See the LICENSE file
//// Created     : Jul 24, 2017
////============================================================================
//
//#include "ibex_CellBufferOptim.h"
//
//namespace ibex {
//
//CellBufferOptimDefault::CellBufferOptimDefault(const System& sys, int crit2_pr, CellCostFunc::criterion crit2) :
//		CellDoubleHeap(*new CellCostVarLB(n), *CellCostFunc::get_cost(crit2, sys.nb_var), crit2_pr),
//		sys(sys) {
//
//}
//
//CellBufferOptimDefault::~CellBufferOptimDefault() {
//	flush();
//	delete &cost1();
//	delete &cost2();
//}
//
//void CellBufferOptimDefault::add_backtrackable(Cell& root) {
//	// add data "pu" and "pf" (if required)
//	cost2().add_backtrackable(root);
//}
//
///** Push a new cell on the stack. */
//void CellBufferOptimDefault::push(Cell* cell) {
//	// we know cost1() does not require OptimData
//	cost2().set_optim_data(*cell,sys);
//
//	// the cell is put into the 2 heaps
//	CellDoubleHeap::push(cell);
//}
//
//} /* namespace ibex */
