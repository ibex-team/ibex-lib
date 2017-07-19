//============================================================================
//                                  I B E X
// File        : test_ampl
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Dec 4, 2013
// Last Update : Dec 4, 2012
//============================================================================


#include "ibex.h"

#ifndef _IBEX_WITH_AMPL_
#error "You need the plugin AMPL to run this example."
#endif

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

	cout<<"begin"<< endl;
	AmplInterface interface(argv[1]);
	cout<<"creation ok"<< endl;
	System sys(interface);
	cout<<"Bound_sys"<< endl;
	cout<<sys.box<<endl;
	cout<<sys<<endl;

/*
	ExtendedSystem ext_sys(*sys,1.e-6);
	cout<<"Extended System"<< endl;
	cout<<ext_sys<<endl;
*/
	System sys2(argv[2]);
	cout<<"Bound_sys"<< endl;
	cout<<sys2.box<<endl;
	cout<<sys2<<endl;

/*
	ExtendedSystem ext_sys2(sys2,1.e-6);
	cout<<"Extended System"<< endl;
	cout<<ext_sys2<<endl;

*/

	return 0;

}

