//============================================================================
//                                  I B E X                                   
// File        : ibex_CellBuffer.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 15, 2012
// Last Update : May 15, 2012
//============================================================================

#include "ibex_CellBuffer.h"

using namespace std;

namespace ibex {

CellBuffer::CellBuffer() : capacity(-1), screen(1) { }

CellBuffer::~CellBuffer() { }

std::ostream& operator<<(std::ostream& os, const CellBuffer& buffer) {
	os << "==============================================================================\n";
	os << "[" << buffer.screen++ << "] buffer size=" << buffer.size() << " . Cell on the top :\n\n ";

	os << buffer.top()->box;

	return os << endl;
}

}  // end namespace ibex
