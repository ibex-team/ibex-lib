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

extern int ibexparse();
extern FILE* ibexin;
extern void ibexrestart(FILE *);

namespace ibex {

namespace parser {
extern System* system;
}

System::System(const char* filename) : box(1), nb_var(0), nb_ctr(0) /* tmp */ {
	FILE *fd;
	if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);
	load(fd);
}

System::System(int n, const char* syntax) : box(1),    /* tmp */
		                                    nb_var(n), /* NOT TMP (required by parser) */
		                                    nb_ctr(0)  /* tmp */ {
	FILE* fd=tmpfile ();

	for (int i=0; i<strlen(syntax); i++)
		putc(syntax[i],fd);
	load(fd);
}

void System::load(FILE* fd) {
	ibexin = fd;

	try {
		parser::system=this;
		ibexparse();
	}

	catch(SyntaxError& e) {
		fclose(fd);
		//clean();
		ibexrestart(ibexin);
		throw e;
	}

	fclose(fd);
}

System::~System() {
	for (int i=0; i<func.size(); i++)
		delete &func[i];

	if (goal) delete &goal;

	for (int i=0; i<ctrs.size(); i++)
		delete &ctrs[i];
}

} // end namespace ibex
