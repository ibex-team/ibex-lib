//============================================================================
//                                  I B E X                                   
// File        : ibex_CellBuffer.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 15, 2012
// Last Update : May 15, 2012
//============================================================================

#include "ibex_CellBuffer.h"

using namespace std;

namespace ibex {

CellBuffer::CellBuffer() : capacity(-1), screen(1) { }

CellBuffer::~CellBuffer() { }

std::ostream& CellBuffer::print(std::ostream& os) const{
	os << "==============================================================================\n";
	os << "[" << screen++ << "] buffer size=" << size() << " . Cell on the top :\n\n ";

	os << top()->box;

	return os << endl;
}

std::ostream& operator<<(std::ostream& os, const CellBuffer& h) {
	return  h.print(os);
}


}  // end namespace ibex
