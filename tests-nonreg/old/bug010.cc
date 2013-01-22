#include "IbexDefaultSolver.h"
#include "IbexSystem.h"

int main(int argc, char* argv[]) {
 IbexSystem sys("ponts.txt");
 IbexDefaultSolver solver(sys);
 solver.certification=true;
 solver.explore();
 cout << solver.bflag(0) << endl;
 return 0;
}
/* must display "2" */
