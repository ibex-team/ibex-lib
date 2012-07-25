//============================================================================
//                                  I B E X                                   
// File        : ibex_System.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_System.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"
#include <stdio.h>
#include <sstream>

extern int ibexparse();
extern void ibexparse_string(const char* syntax);
//extern int ibex_lineno;
extern void ibexrestart(FILE *);
extern FILE* ibexin;

namespace ibex {

namespace parser {
extern System* system;
}

System::System(const char* filename) : nb_var(0), nb_ctr(0), box(1) /* tmp */ {
	FILE *fd;
	if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);
	load(fd);
}

System::System(int n, const char* syntax) : nb_var(n), /* NOT TMP (required by parser) */
		                                    nb_ctr(0), box(1) /* tmp */ {
	try {
		parser::system=this;
		ibexparse_string(syntax);
	} catch(SyntaxError& e) {
		throw e;
	}
}

void System::load(FILE* fd) {
	ibexin = fd;

	try {
		parser::system=this;
		ibexparse();
	}

	catch(SyntaxError& e) {
		fclose(fd);
		ibexrestart(ibexin);
		throw e;
	}

	fclose(fd);
}

System::~System() {
	for (int i=0; i<func.size(); i++)
		delete &func[i];

	if (goal) delete goal;

	for (int i=0; i<ctrs.size(); i++)
		delete &ctrs[i];
}

} // end namespace ibex
