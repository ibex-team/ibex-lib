
#include "IbexQuimper.h"
#include "IbexException.h"

using namespace ibex;


bool bug020() {

  try {
    QuimperParser q("20/scal.qpr");
    return true;

  } catch(NonRecoverableException e) {
    cout << e <<endl;
    return false;
  }
}


