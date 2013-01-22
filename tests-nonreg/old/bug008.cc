
#include "IbexDefaultSolver.h"
#include "IbexSystem.h"
#include "IbexBisection.h"
#include "IbexSystemBuilder.h"

int main(int argc, char* argv[]) {

  /* first memory leak problem (destructor of class IbexSystem uncomplete) */
  IbexSystem sys("ponts.txt");

  /* other memory leak problem, when build() is not called
     with an IbexDefaultBuilder object */
  IbexSymbol x("x");
  IbexRepository rep;
  rep.add(x=0);
  IbexDefaultBuilder builder(rep);
  builder.set_entity(0,IBEX_VAR);
}

/**
 * valgrind --leak-check=full ./bug008
 * ===>  LEAK SUMMARY:
 *   definitely lost: 0 bytes in 0 blocks.
 *    possibly lost: 0 bytes in 0 blocks.
 */
