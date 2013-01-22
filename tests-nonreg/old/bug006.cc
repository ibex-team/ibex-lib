#include "IbexSystem.h"

int main(int argc, char* argv[]) {
  IbexSymbol x("x");        
  const IbexExpr& exr(x=0);
}
/* must compile */
