#include "IbexDefaultSolver.h"
#include "IbexSystem.h"

int main(int argc, char* argv[]) {
  try {
    IbexSystem sys("ponts");
  } catch (IbexUnknownFileException e) {
    cerr << e << endl;
  }
  return 0;
}
/* 1- must display "could not open file ponts" */

/* 2- valgrind --leak-check=full ./solve-test ponts
   must result in 0 "possibly lost" bytes */
