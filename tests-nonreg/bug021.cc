#include "IbexQuimper.h"
#include "IbexException.h"

using namespace ibex;


bool bug021() {

  try {
    QuimperParser q("21/scal.qpr");
    return true;

  } catch(NonRecoverableException e) {
    cout << e <<endl;
    return false;
  }
}


