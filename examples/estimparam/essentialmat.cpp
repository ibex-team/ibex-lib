#include "ibex.h"
using namespace std;
using namespace ibex;

 int main(int argc, char** argv) {
   Variable v(3,3);
   try {

   Function essential("essentialmatrix.txt");
   }
   catch (SyntaxError e)
     {cout <<e << endl;}
 }
