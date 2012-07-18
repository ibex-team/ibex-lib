//============================================================================
//                                  I B E X                                   
// File        : ibex_String.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : Jul 18, 2012
//============================================================================

#include "ibex_String.h"

namespace ibex {

int index_2_string(char* buff, char lbracket, char rbracket, int index) {
	assert(index<1000000);
	int k=0;
	char number[6];
	buff[k++]=lbracket;
	snprintf(number, 6, "%d", index);
	strcpy(&buff[k], number);
	k+=strlen(number);
	buff[k++]=rbracket;
	buff[k]='\0';
	return k;
}

} // end namespace ibex
