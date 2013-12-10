//============================================================================
//                                  I B E X
// File        : test_ampl
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Dec 4, 2013
// Last Update : Dec 4, 2012
//============================================================================


#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {
	cout<<"begin"<< endl;
	AmplInterface interface(argv[1]);
	cout<<"creation ok"<< endl;
	System sys =interface.getSystem();

	cout<<"Goal"<< endl;
	cout<<*(sys.goal)<< endl;

	cout<<"Constraints"<< endl;
	cout<<sys.ctrs[0]<< endl;
	cout<<sys.ctrs[1]<< endl;


	return 0;

}

